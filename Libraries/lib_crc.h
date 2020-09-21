/******************************************************************************
 * @File: lib_crc.h
 * @Author: Milandr, L.
 * @Project: Library
 * @Version: 1.0.0
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 26.08.2020
 * @Purpose: Реализация алгоритмов расчёта циклического избыточного кода (CRC)
 ******************************************************************************/

// Условная компиляция
#ifndef __LIB_CRC_H
#define __LIB_CRC_H

// Подключение заголовков
#include <stdint.h>
#include <stdio.h>

// Прототипы функций
uint32_t CRC32(uint8_t *data, size_t size);

#endif
