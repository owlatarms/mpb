/******************************************************************************
 * @File: lib_sort.h
 * @Author: Milandr, L.
 * @Project: Library
 * @Version: 1.1.0
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 31.03.2020
 * @Purpose: Функции сортировки
 ******************************************************************************/

// Условная компиляция
#ifndef __LIB_SORT_H
#define __LIB_SORT_H

// Подключение библиотеки микроконтроллера
#include "1986VE9x.h"

// Подключение стандартной библиотеки языка Си
#include <stdlib.h>

// Прототипы функций
void BubbleSort(int *sample, size_t size);
void CombSort(int *sample, size_t size);
void InsertSort(int *sample, size_t size);
void ShellSort(int *sample, size_t size);
void QuickSort(int *sample, size_t size);
bool Validate(int *sample, size_t size);
void Randomize(int *sample, size_t size, uint32_t lim);

#endif
