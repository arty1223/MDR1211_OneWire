/*
 * onewire.c
 *
 *  Created on: Jul 11, 2025
 *      Authors: Artyom Skibitskiy
 *      Реализация OneWire на базе UART.
 *
 */

#include "onewire.h"

static PORT_InitTypeDef OneWire_PortInit;
static UART_InitTypeDef OneWire_UART_InitStructure;

void initOneWire(void)
{

    RST_CLK_PCLKcmd((RST_CLK_PCLK_PORTB | RST_CLK_PCLK_UART1), ENABLE);

    PORT_DeInit(MDR_PORTB);

    /* Configure PORTB pins 5 (UART1_TX) as output */
    PORT_StructInit(&OneWire_PortInit);
    OneWire_PortInit.PORT_Pin = PORT_Pin_5;
    OneWire_PortInit.PORT_FUNC = PORT_FUNC_ALTER;
    OneWire_PortInit.PORT_SPEED = PORT_SPEED_MAXFAST;
    OneWire_PortInit.PORT_MODE = PORT_MODE_DIGITAL;
    OneWire_PortInit.PORT_OE = PORT_OE_OUT;
    OneWire_PortInit.PORT_PD = PORT_PD_OPEN;
    PORT_Init(MDR_PORTB, &OneWire_PortInit);

    /* Configure PORTB pins 6 (UART1_RX) as input */
    OneWire_PortInit.PORT_Pin = PORT_Pin_6;
    OneWire_PortInit.PORT_OE = PORT_OE_IN;
    PORT_Init(MDR_PORTB, &OneWire_PortInit);

    UART_DeInit(MDR_UART1);

    /* Set the HCLK division factor = 1 for UART1,UART2 */
    UART_BRGInit(MDR_UART1, UART_HCLKdiv8);

    /* Initialize OneWire_UART_InitStructure */
    OneWire_UART_InitStructure.UART_BaudRate = 9600 >> 1;
    OneWire_UART_InitStructure.UART_WordLength = UART_WordLength8b;
    OneWire_UART_InitStructure.UART_StopBits = UART_StopBits1;
    OneWire_UART_InitStructure.UART_Parity = UART_Parity_1;
    OneWire_UART_InitStructure.UART_FIFOMode = UART_FIFO_ON;
    OneWire_UART_InitStructure.UART_HardwareFlowControl = (UART_HardwareFlowControl_RXE |
                                                           UART_HardwareFlowControl_TXE);

    /* Configure UART1 parameters */
    UART_Init(MDR_UART1, &OneWire_UART_InitStructure);

    // /* Configure DMA for UART1 */
    UART_DMAConfig(MDR_UART1, UART_IT_FIFO_LVL_12words, UART_IT_FIFO_LVL_12words);
    UART_DMACmd(MDR_UART1, (UART_DMA_TXE | UART_DMA_RXE | UART_DMA_ONERR), ENABLE);

    /* Enables UART1 peripheral */
    UART_Cmd(MDR_UART1, ENABLE);
}

uint8_t OneWire_Reset(void)
{
    uint8_t ResetByte = 0xF0, PresenceByte;
    UART_DeInit(MDR_UART1);
    OneWire_UART_InitStructure.UART_BaudRate = 9600 >> 1;
    UART_Init(MDR_UART1, &OneWire_UART_InitStructure);
    UART_Cmd(MDR_UART1, ENABLE);
    UART_SendData(MDR_UART1, ResetByte);
    while (UART_GetFlagStatus(MDR_UART1, UART_FLAG_BUSY))
    {
    }

    // UART_ReceiveData(MDR_UART1);
    // Wait for the presence pulse
    PresenceByte = (uint8_t)UART_ReceiveData(MDR_UART1);
    OneWire_UART_InitStructure.UART_BaudRate = 115200 >> 1;
    UART_Init(MDR_UART1, &OneWire_UART_InitStructure);
    UART_Cmd(MDR_UART1, ENABLE);
    while (UART_GetFlagStatus(MDR_UART1, UART_FLAG_BUSY))
    {
    }
    // Check presence pulse
    if (PresenceByte != ResetByte)
    {
        return 1; // Presence pulse detected
    }
    else
    {
        return 0; // No presence pulse detected
    }
}

uint8_t OneWire_ReadBit(void)
{
    #define ReadBitCMD 0xFF

    // Send Read Bit CMD
    UART_SendData(MDR_UART1, ReadBitCMD);
    while (UART_GetFlagStatus(MDR_UART1, UART_FLAG_BUSY))
    {
    }
    // Receive The Bit
    uint8_t RxBit = (uint8_t)UART_ReceiveData(MDR_UART1);

    return (RxBit & 0x01);
}

uint8_t OneWire_ReadByte(void)
{
    uint8_t RxByte = 0;
    for (uint8_t i = 0; i < 8; i++)
    {
        RxByte |= OneWire_ReadBit() << i;
    }
    return RxByte;
}

void OneWire_WriteBit(uint8_t Bit)
{
    UART_SendData(MDR_UART1, (Bit ? 0xFF : 0x00));
    while (UART_GetFlagStatus(MDR_UART1, UART_FLAG_BUSY))
    {
    }
    UART_ReceiveData(MDR_UART1); // Так как работаем в полудуплексном режиме, необходимо очистить буфер после отправки бита
}

void OneWire_WriteByte(uint8_t data)
{
    uint8_t TxBuffer[8];
    for (int i = 0; i < 8; i++)
    {
        OneWire_WriteBit((data & (1 << i)));
    }
}
