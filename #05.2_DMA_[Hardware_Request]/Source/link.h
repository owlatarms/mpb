/******************************************************************************
 * @File: main.c
 * @Author: Milandr, L.
 * @Project: #05.2_DMA_[Hardware_Request]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 07.05.2020
 * @Purpose: Связывание модулей проекта
 ******************************************************************************/

// Условная компиляция
#ifndef __LINK_H
#define __LINK_H

// Подключение стандартной библиотеки языка Си
#include <stdlib.h>

// Подключение библиотеки микроконтроллера
#include <1986VE9x.h>

// Подключение заголовков модулей проекта
#include <lib_clk.h>
#include <lib_tick.h>
#include <lib_lcd.h>
#include <lib_sort.h>
#include "dma.h"
#include "tmr.h"
#include "arr.h"

#endif
