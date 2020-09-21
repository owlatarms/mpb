/******************************************************************************
 * @File: main.c
 * @Author: Milandr, L.
 * @Project: #06.2_DAC_[Cyclic_Conversion]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 19.05.2020
 * @Purpose: Связывание модулей проекта
 ******************************************************************************/

// Условная компиляция
#ifndef __LINK_H
#define __LINK_H

// Подключение библиотеки микроконтроллера
#include <1986VE9x.h>

// Подключение стандартной библиотеки языка Си
#include <stdlib.h>

// Подключение библиотеки математических операций
#include <math.h>

// Подключение заголовков модулей проекта
#include <lib_clk.h>
#include <lib_tick.h>
#include <lib_btn.h>
#include <lib_lcd.h>
#include "dac.h"
#include "tmr.h"
#include "dma.h"

#endif
