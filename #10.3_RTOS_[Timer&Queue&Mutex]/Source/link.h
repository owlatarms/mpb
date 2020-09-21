/******************************************************************************
 * @File: link.h
 * @Author: Milandr, L.
 * @Project: #10.3_RTOS_[Timer&Queue&Mutex]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 22.06.2020
 * @Purpose: Связывание модулей проекта
 ******************************************************************************/

// Условная компиляция
#ifndef __LINK_H
#define __LINK_H

// Подключение библиотеки микроконтроллера
#include <1986VE9x.h>

// Подключение библиотек ОСРВ
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include <timers.h>
#include <event_groups.h>

// Подключение заголовков модулей проекта
#include <lib_clk.h>
#include <lib_btn.h>
#include "main.h"
#include "led.h"
#include "rtos.h"

#endif
