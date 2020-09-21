/******************************************************************************
 * @File: main.c
 * @Author: Milandr, L.
 * @Project: #12.2_SPI_[DMA_Exchange]
 * @Version: 1.0.0
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 06.08.2020
 * @Purpose: Основной модуль
 * @Description:
 * В данном проекте реализована работа с внешним перепрограммируемым ПЗУ через
 * интерфейс SPI и DMA. В качестве записываемых данных выступает произвольная
 * строка. Считанные данные отображаются на дисплей. Управление процессом
 * организовано с помощью кнопок:
 * - UP: запись данных;
 * - DOWN: чтение данных;
 * - SENSE: стирание данных.
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
  size_t data_size = sizeof(EX_STR) + 1;  // Объём передаваемых данных
  uint32_t eeprom_address = 0x00000;      // Адрес в пространстве ПЗУ

  // Формирование строки для передачи
  strcpy((char*)&SPI_TX_BUF, EX_STR);

  // Основной цикл
  while (true) {

    // Блок работы с записью данных
    if (Wait(&mark[0], 100) == true) {

      // Обработка кнопки UP
      if (BTN_Handler(BTN_U) == true) {

        // Передача (запись) данных
        EEPROM_WriteData(eeprom_address, (uint8_t*)SPI_TX_BUF, data_size);
      }
    }

    // Блок работы с чтением данных
    if (Wait(&mark[1], 100) == true) {

      // Обработка кнопки MID
      if (BTN_Handler(BTN_M) == true) {

        // Приём (чтение) данных
        EEPROM_ReadData(eeprom_address, (uint8_t*)SPI_RX_BUF, data_size);

        // Очистка дисплея (строки 3...6)
        for (int i = 3; i < 7; i++) {
          LCD_ClearString(i);
        }

        // Отображение данных
        LCD_PrintString((char*)SPI_RX_BUF, 3);
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
