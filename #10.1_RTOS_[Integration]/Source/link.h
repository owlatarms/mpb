/******************************************************************************
 * @File: link.h
 * @Author: Milandr, L.
 * @Project: #10.1_RTOS_[Integration]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 04.06.2020
 * @Purpose: Связывание модулей проекта
 ******************************************************************************/

// Условная компиляция
#ifndef __LINK_H
#define __LINK_H

// Подключение библиотеки микроконтроллера
#include <1986VE9x.h>

// Подключение заголовков модулей проекта
#include <lib_clk.h>
#include <lib_btn.h>
#include "main.h"
#include "led.h"
#include "rtos.h"

#endif
