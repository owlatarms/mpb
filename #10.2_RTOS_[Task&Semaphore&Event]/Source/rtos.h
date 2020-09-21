/******************************************************************************
 * @File: rtos.h
 * @Author: Milandr, L.
 * @Project: #10.2_RTOS_[Task&Semaphore&Event]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 09.06.2020
 * @Purpose: Связывание модулей проекта
 ******************************************************************************/

// Условная компиляция
#ifndef __RTOS_H
#define __RTOS_H

// Подключение заголовков
#include "link.h"

// Прототипы функций
void RTOS_Init(void);

// Кроссмодульные переменные
extern TaskHandle_t id_task_led_start;
extern TaskHandle_t id_task_led_pause;
extern TaskHandle_t id_task_led_blink[LED_NUM];
extern EventGroupHandle_t id_event_led;
extern SemaphoreHandle_t id_semaphore_led;

#endif
