# Реализация OneWire на базе UART
### Функционал:
``` C
void initOneWire(void); // Инициализация OneWire на портах PB5 PB6. PB5 и PB6 должны быть соединены между собой.
uint8_t OneWire_Reset(void); // Сброс шины OneWire (Используется для начала работы с устройствами на шине)

// Функции для работы с OneWire
uint8_t OneWire_ReadBit(void); // Чтение бита из шины OneWire
uint8_t OneWire_ReadByte(void); // Чтение байта из шины OneWire
void OneWire_WriteBit(uint8_t); // Запись бита в шину OneWire
void OneWire_WriteByte(uint8_t); // Запись байта в шину OneWire
```

### Зависимости (Поставляются вместе с дистрибутивом SPL на сайте Миландр):
- "MDR32FxQI_port.h"
- "MDR32FxQI_uart.h"

*note: В данном случае подразумевается, что зависимости подключаются в файле app.h согласно структуры [проекта-шаблона](https://github.com/arty1223/MILUINO/tree/main/Milandr_GCC_template). Примером использования библиотеки и настройки зависимостей служит [библиотека для DS18B20](https://github.com/arty1223/MDR1211_DS18B20).*

PS: Реализовано на базе UART1 на пинах PB5 и PB6. Для работы с UART2 или на других пинах необходимо изменить функцию initOneWire() под ваши нужды, а также заменить в коде все упоминания MDR_UART1 на MDR_UART2, если вы используете UART2.