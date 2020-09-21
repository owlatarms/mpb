/******************************************************************************
 * @File: arr.h
 * @Author: Milandr, L.
 * @Project: #05.1_DMA_[Software_Request]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 30.04.2020
 * @Purpose: Работа с массивами
 ******************************************************************************/

// Условная компиляция
#ifndef __ARR_H
#define __ARR_H

// Подключение заголовков
#include "link.h"

// Размер массивов
#define ARR_S 16

// Максимальное значение п/случайной последовательности
#define RND_MAX 999

// Кроссмодульные переменные
extern volatile int source_array[ARR_S];
extern volatile int result_array[ARR_S];

// Прототипы функций
void PrintElement(int element, uint8_t x, uint8_t y);
void PrintArray(volatile int *array);

#endif
