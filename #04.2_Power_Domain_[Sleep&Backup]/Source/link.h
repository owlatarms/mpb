/******************************************************************************
 * @File: link.h
 * @Author: Milandr, L.
 * @Project: #04.2_Power_Domain_[Sleep&Backup]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 29.04.2020
 * @Purpose: Связывание модулей проекта
 ******************************************************************************/

// Условная компиляция
#ifndef __LINK_H
#define __LINK_H

// Подключение библиотеки микроконтроллера
#include <1986VE9x.h>

// Подключение библиотеки меток времени
#include <time.h>

// Подключение заголовков модулей проекта
#include <lib_clk.h>
#include <lib_tick.h>
#include <lib_btn.h>
#include <lib_lcd.h>
#include "main.h"
#include "bkp.h"
#include "led.h"

#endif
