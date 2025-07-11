/*
 * onewire.h
 *
 *  Created on: Jul 11, 2025
 *      Authors: Artyom Skibitskiy
 *      Реализация OneWire на базе UART.
 *
 */
#ifndef INC_ONEWIRE_H_
#define INC_ONEWIRE_H_

#pragma once
#include "app.h"


void initOneWire(void); // инициализация OneWire на портах PB5 PB6. PB5 и PB6 должны быть соединены между собой.
uint8_t OneWire_Reset(void);
uint8_t OneWire_ReadBit(void);
uint8_t OneWire_ReadByte(void);
void OneWire_WriteBit(uint8_t);
void OneWire_WriteByte(uint8_t);

#endif /* INC_ONEWIRE_H_ */