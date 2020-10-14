/******************************************************************************
 * @File: link.h
 * @Author: Milandr, L.
 * @Project: #08.2_PWM_[Variable_Frequency]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 02.04.2020
 * @Purpose: Связывание модулей проекта
 ******************************************************************************/

// Условная компиляция
#ifndef __LINK_H
#define __LINK_H

// Подключение библиотеки микроконтроллера
#include <1986VE9x.h>

// Подключение заголовков модулей проекта
#include <lib_clk.h>
#include <lib_tick.h>
#include <lib_btn.h>
#include <lib_lcd.h>
#include <lib_sort.h>
#include "main.h"
#include "pwm.h"
#include "adc.h"
#include "dma.h"

#endif
