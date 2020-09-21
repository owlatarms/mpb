/******************************************************************************
 * @File: rtos.c
 * @Author: Milandr, L.
 * @Project: #10.3_RTOS_[Timer&Queue&Mutex]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 22.06.2020
 * @Purpose: Инициализация операционной системы реального времени
 ******************************************************************************/

// Подключение заголовка
#include "rtos.h"

// Идентификаторы задач
TaskHandle_t id_task_led_track = NULL;
TaskHandle_t id_task_led_pause = NULL;

// Идентификатор таймера
TimerHandle_t id_timer_led = NULL;

// Идентификатор очереди
QueueHandle_t id_queue_led = NULL;

// Идентификатор семафора
SemaphoreHandle_t id_semaphore_led = NULL;

// Инициализация ОСРВ
void RTOS_Init(void)
{
  // Создание задачи отслеживания состояния светодиодов
  xTaskCreate(TaskLedTrack, NULL, 80, NULL, 3, &id_task_led_track);

  // Создание задачи остановки запуска светодиодов
  xTaskCreate(TaskLedPause, NULL, 52, NULL, 3, &id_task_led_pause);

  // Создание таймера
  id_timer_led = xTimerCreate(NULL, BLINK_DELAY, pdTRUE, NULL, CallbackLedBlink);

  // Создание очереди
  id_queue_led = xQueueCreate(QUEUE_LEN, QUEUE_WID);

  // Создание мьютекса
  id_semaphore_led = xSemaphoreCreateMutex();

  // Запуск таймера
  xTimerStart(id_timer_led, 0);

  // Запуск диспетчера
  vTaskStartScheduler();
}
