/******************************************************************************
 * @File: main.c
 * @Author: Milandr, L.
 * @Project: #10.2_RTOS_[Task&Semaphore&Event]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 09.06.2020
 * @Purpose: Основной модуль
 * В данном проекте показана работа с сервисами синхронизации операционной системы -
 * событиями и семафорами; одна задача последовательно запускает другие, при этом
 * процесс запуска может быть приостановлен нажатием кнопки. Также показан способ
 * создания нескольких задач по одному шаблону с различными аргументами.
 ******************************************************************************/

// Подключение заголовка
#include "main.h"

// Основная функция
int main(void)
{
  // Общая инициализация
  CPU_Init();   // Система тактирования
  BTN_Init();   // Кнопки

  // Инициализация светодиодов
  LED_Init();

  // Инициализация ОСРВ
  RTOS_Init();

  // Основной цикл
  // (недосягаемый фрагмент!)
  while (true) {
  }
}

// Задача переключения светодиодов (шаблон)
void TaskLedBlink(void *arg)
{
  // Извлечение данных из аргумента задачи
  // (обратите внимание на преобразование типов)
  uint8_t led_num = *(uint8_t *)arg;

  // Основной цикл
  while (true) {

    // Ожидание события, т.е. разрешения на работу потока
    xEventGroupWaitBits(id_event_led, led_num, pdTRUE, pdTRUE, portMAX_DELAY);

    // Переключение светодиода
    MDR_PORTA->RXTX ^= led_num;

    // Передача семафора
    xSemaphoreGive(id_semaphore_led);
  }
}

// Задача синхронизированного запуска светодиодов
void TaskLedStart(void *arg)
{
  // Счётчик семафоров
  uint8_t semaphore = 0;

  // Основной цикл
  while (true) {

    // Задержка перед первым запуском
    vTaskDelay(BLINK_DELAY);

    // Цикл последовательного запуска задач
    for (uint8_t i = 0; i < LED_NUM; i++) {

      // Установка битов события для запуска потока
      xEventGroupSetBits(id_event_led, (0x01 << i));

      // Задержка перед следующим запуском
      vTaskDelay(BLINK_DELAY);

      // Захват семафора
      semaphore += xSemaphoreTake(id_semaphore_led, 0);
    }

    // Если все потоки передали семафоры...
    if (semaphore == LED_NUM) {

      // Феерверк (3 цикла)
      for (uint8_t i = 0; i < 3 * 2; i++) {

        // Перключение светодиодов
        MDR_PORTA->RXTX ^= 0xFF;

        // Задержка
        vTaskDelay(100);
      }
    }

    // Отключение светодиодов
    MDR_PORTA->RXTX &= ~0xFF;

    // Обнуление счётчика семафоров
    semaphore = 0;
  }
}

// Задача остановки запуска светодиодов
void TaskLedPause(void *arg)
{
  // Основной цикл
  while (true) {

    // Обработки кнопки MID
    if (BTN_Handler(BTN_M) == true) {

      // Если задача запущена...
      if (eTaskGetState(id_task_led_start) != eSuspended) {

        // Остановка задачи
        vTaskSuspend(id_task_led_start);

      }

      // Иначе - если задача остановлена...
      else {

        // Запуск задачи
        vTaskResume(id_task_led_start);
      }
    }

    // Задержка перед началом следующей обработки
    vTaskDelay(100);
  }
}
