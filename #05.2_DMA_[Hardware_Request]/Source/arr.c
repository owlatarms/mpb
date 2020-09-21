/******************************************************************************
 * @File: arr.h
 * @Author: Milandr, L.
 * @Project: #05.2_DMA_[Hardware_Request]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 07.05.2020
 * @Purpose: Работа с массивами
 ******************************************************************************/

// Подключение заголовка
#include "arr.h"

// Инициализация переменных
volatile int source_array[ARR_S] = {0};  // Массив исходных данных
volatile int result_array[ARR_S] = {0};  // Массив принятых данных

// Отображение элемента массива
// (из расчёта на трехзначное десятичное число)
void PrintElement(int element, uint8_t x, uint8_t y)
{
  // Массив десятичных значений числа
  char digit[3 + 1] = {0};

  // Преобразование числа в строку
  snprintf(digit, 3 + 1, "%03u", element);

  // Цикл отображения элемента по заданным координатам
  for (int i = 0; i < 3; i ++) {
    LCD_PrintSymbol(digit[i], x + i, y);
  }
}

// Отображение массива
void PrintArray(volatile int *array)
{
  // Инициализация переменных
  static uint8_t stored_array[ARR_S] = {0};  // Массив отображённых данных
  static bool first_call = true;             // Флаг первого вызова функции
         uint8_t x = 0, y = 0;               // Координаты элемента

  // Цикл отображения данных
  for (int i = 0; i < ARR_S; i++) {

    // Проверка отличия текущего и отображённого элементов массива
    // (выполняется, чтобы не отображать одни и те же данные)
    if (stored_array[i] != array[i] || first_call != false) {

      // Копирование элемента
      stored_array[i] = array[i];

      // Вычисление координат
      x = 3 + (i % 4) * 4;
      y = i / 4 + 3;

      // Отображение элемента массива на дисплей
      PrintElement(array[i], x, y);
    }
  }

  // Сброс флага первого вызова функции
  first_call = false;
}
