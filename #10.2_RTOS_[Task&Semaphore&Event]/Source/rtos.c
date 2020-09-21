/******************************************************************************
 * @File: rtos.c
 * @Author: Milandr, L.
 * @Project: #10.2_RTOS_[Task&Semaphore&Event]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 09.06.2020
 * @Purpose: Инициализация операционной системы реального времени
 ******************************************************************************/

// Подключение заголовка
#include "rtos.h"

// Идентификаторы задач
TaskHandle_t id_task_led_start = NULL;
TaskHandle_t id_task_led_pause = NULL;
TaskHandle_t id_task_led_blink[LED_NUM] = {NULL};

// Идентификатор группы событий
EventGroupHandle_t id_event_led = NULL;

// Идентификатор семафора
SemaphoreHandle_t id_semaphore_led = NULL;

// Инициализация ОСРВ
void RTOS_Init(void)
{
  // Массив параметров задач
  static uint8_t led[LED_NUM] = {0};

  // Создание задачи запуска переключений
  xTaskCreate(TaskLedStart, NULL, 44, NULL, 2, &id_task_led_start);

  // Создание задачи остановки запуска
  xTaskCreate(TaskLedPause, NULL, 28, NULL, 3, &id_task_led_pause);

  // Цикл создания задач мигания
  for (uint8_t i = 0; i < LED_NUM; i++) {

    // Формирование параметра задачи
    led[i] = 0x01 << i;

    // Создание задачи
    xTaskCreate(TaskLedBlink, NULL, 60, &led[i], 1, &id_task_led_blink[i]);
  }

  // Создание группы событий
  id_event_led = xEventGroupCreate();

  // Создание счётного семафора
  id_semaphore_led = xSemaphoreCreateCounting(LED_NUM, 0);

  // Запуск диспетчера
  vTaskStartScheduler();
}
