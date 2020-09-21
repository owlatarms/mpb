/******************************************************************************
 * @File: rtos.h
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
#ifndef __RTOS_H
#define __RTOS_H

// Подключение заголовков
#include "link.h"

// Параметры очереди
#define QUEUE_LEN 1                // Максимальная длина
#define QUEUE_WID sizeof(uint8_t)  // Максимальная ширина (размер одного элемента)

// Прототипы функций
void RTOS_Init(void);

// Кроссмодульные переменные
extern TaskHandle_t id_task_led_track;
extern TaskHandle_t id_task_led_pause;
extern TimerHandle_t id_timer_led;
extern QueueHandle_t id_queue_led;
extern SemaphoreHandle_t id_semaphore_led;

#endif
