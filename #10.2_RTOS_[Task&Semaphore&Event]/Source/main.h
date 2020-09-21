/******************************************************************************
 * @File: main.h
 * @Author: Milandr, L.
 * @Project: #10.2_RTOS_[Task&Semaphore&Event]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 09.06.2020
 * @Purpose: Основной модуль
 ******************************************************************************/

// Условная компиляция
#ifndef __MAIN_H
#define __MAIN_H

// Кол-во состояний светодиодов
#define LED_NUM 8

// Период переключения светодиодов (в миллисекундах)
#define BLINK_DELAY 500

// Подключение заголовков
#include "link.h"

// Прототипы функций
void TaskLedBlink(void *arg);
void TaskLedStart(void *arg);
void TaskLedPause(void *arg);

#endif
