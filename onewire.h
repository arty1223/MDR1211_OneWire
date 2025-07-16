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


void initOneWire(void); // Инициализация OneWire на портах PB5 PB6. PB5 и PB6 должны быть соединены между собой.
uint8_t OneWire_Reset(void); // Сброс шины OneWire (Используется для начала работы с устройствами на шине)

// Функции для работы с OneWire
uint8_t OneWire_ReadBit(void); // Чтение бита из шины OneWire
uint8_t OneWire_ReadByte(void); // Чтение байта из шины OneWire
void OneWire_WriteBit(uint8_t); // Запись бита в шину OneWire
void OneWire_WriteByte(uint8_t); // Запись байта в шину OneWire

#endif /* INC_ONEWIRE_H_ */