/******************************************************************************
 * @File: main.c
 * @Author: Milandr, L.
 * @Project: #12.1_SPI_[Basic_Exchange]
 * @Version: 1.1.0
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 26.08.2020
 * @Purpose: Основной модуль
 * @Description:
 * В данном проекте реализована работа с внешним перепрограммируемым ПЗУ через
 * интерфейс SPI. В качестве записываемых данных выступает строка. Управление
 * процессом организовано с помощью кнопок:
 * - UP: запись данных;
 * - DOWN: чтение данных;
 * - SENSE: стирание данных.
 * При каждой операции вычисляется контрольная сумма. Критерием успешного обмена
 * является равенство контрольных сумм.
 ******************************************************************************/

// Подключение заголовка
#include "main.h"

// Основная функция
int main(void)
{
  // Общая инициализация
  CPU_Init();   // Система тактирования
  TICK_Init();  // Системный таймер
  BTN_Init();   // Кнопки
  LCD_Init();   // ЖК-дисплей

  // Инициализация SPI
  SPI_Init();

  // Инициализация переменных
  const size_t data_size  = sizeof(EX_STR) + 1;  // Размеры буферов
  char tx_data[data_size] = {EX_STR};            // Буфер передаваемых данных
  char rx_data[data_size] = {0};                 // Буфер принимаемых данных
  uint32_t eeprom_address = 0x00000;             // Адрес в пространстве ПЗУ
  uint32_t crc = 0;                              // Контрольная сумма данных

  // Основной цикл
  while (true) {

    // Блок работы с записью данных
    if (Wait(&mark[0], 100) == true) {

      // Обработка кнопки UP
      if (BTN_Handler(BTN_U) == true) {

        // Передача (запись) данных
        EEPROM_WriteData(eeprom_address, (uint8_t*)tx_data, data_size);

        // Вычисление CRC
        crc = CRC32((uint8_t*)tx_data, data_size);

        // Отображение CRC
        LCD_PrintString(" CRC_TX = 0x%X", 3, crc);
      }
    }

    // Блок работы с чтением данных
    if (Wait(&mark[1], 100) == true) {

      // Обработка кнопки MID
      if (BTN_Handler(BTN_D) == true) {

        // Приём (чтение) данных
        EEPROM_ReadData(eeprom_address, (uint8_t*)rx_data, data_size);

        // Вычисление CRC
        crc = CRC32((uint8_t*)rx_data, data_size);

        // Отображение CRC
        LCD_PrintString(" CRC_RX = 0x%X", 5, crc);
      }
    }

    // Блок работы со стиранием данных
    if (Wait(&mark[2], 100) == true) {

      // Обработка кнопки DOWN
      if (BTN_Handler(BTN_S) == true) {

        // Стирание данных
        for (int i = 0; i < (eeprom_address + (data_size / EEPROM_PAGE_SIZE) + 1); i++) {
          EEPROM_ErasePage(i);
        }
      }
    }

  }
}
