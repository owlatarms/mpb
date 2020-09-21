/******************************************************************************
 * @File: lib_crc.c
 * @Author: Milandr, L.
 * @Project: Library
 * @Version: 1.0.0
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 26.08.2020
 * @Purpose: Реализация алгоритмов расчёта циклического избыточного кода (CRC)
 ******************************************************************************/

// Подключение заголовка
#include "lib_crc.h"

// Вычисление CRC32 (стандарт IEEE 802.3)
uint32_t CRC32(uint8_t *data, size_t size)
{
  // Инициализация переменных
  uint32_t crc_table[8 * 32] = {0};  // Таблица CRC
  uint32_t crc = 0;                  // CRC данных
  const uint32_t poly = 0xEDB88320;  // Полином

  // Формирование таблицы CRC
  for (int i = 0; i < 8 * 32; i++) {

    // Исходное значение элемента
    crc_table[i] = i;

    // Цикл вычисления табличных значений
    for (int j = 0; j < 8; j++) {

      if ((crc_table[i] & 1) == 1) {                // Если младший бит слова равен 1...
        crc_table[i] = (crc_table[i] >> 1) ^ poly;  // Сдвиг вправо и XOR с полиномом
      } else {                                      // Если младший бит слова равен 0...
        crc_table[i] = (crc_table[i] >> 1);         // Сдвиг вправо
      }
    }
  }

  // Вычисление CRC данных
  for (crc = 0xFFFFFFFF; size > 0; size--) {
    crc = (crc >> 8) ^ crc_table[(crc ^ *data++) & 0xFF];
  }

  // Возврат CRC с побитовой инверсией
  return crc ^ 0xFFFFFFFF;
}
