/******************************************************************************
 * @File: main.c
 * @Author: Milandr, L.
 * @Project: #10.3_RTOS_[Timer&Queue&Mutex]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 22.06.2020
 * @Purpose: Основной модуль
 * Данный проект функционально идентичен предыдущему, но алгоритм реализован
 * на таких сервисах ОСРВ, как программные таймеры, очереди и мьютексы.
 * Последовательное переключение светодиодов осуществляется функцией таймера;
 * эта функция передаёт состояние светодиодов через очередь в задачу отслеживания.
 * Когда задача отслеживания фиксирует целевое состояние, она выполняет быстрое
 * мигание светодиодами. Далее начинается новый цикл. Доступ к светодиодом защищён
 * мьютексом для исключения одновременного использования. Как и прежде, весь
 * процесс может быть приостановлен нажатием кнопки.
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

// Задача отслеживания состояния светодиодов
void TaskLedTrack(void *arg)
{
  // Состояние светодиодов
  uint8_t led_state = 0;

  // Основной цикл
  while (true) {

    // Ожидание сообщения
    xQueueReceive(id_queue_led, &led_state, portMAX_DELAY);

    // Если сообщение содержало информацию,
    // что все светодиоды включены...
    if (led_state == 0xFF) {

      // Захват мьютекса
      xSemaphoreTake(id_semaphore_led, portMAX_DELAY);

      // Задержка для фиксации последнего состояния
      vTaskDelay(BLINK_DELAY);

      // Феерверк (3 цикла)
      for (uint8_t i = 0; i < 3 * 2; i++) {

        // Перключение светодиодов
        MDR_PORTA->RXTX ^= 0xFF;

        // Задержка
        vTaskDelay(100);
      }

      // Отключение светодиодов
      MDR_PORTA->RXTX &= ~0xFF;

      // Перезапуск таймера
      xTimerReset(id_timer_led, 0);

      // Освобождение мьютекса
      xSemaphoreGive(id_semaphore_led);
    }
  }
}

// Задача остановки запуска светодиодов
void TaskLedPause(void *arg)
{
  // Основной цикл
  while (true) {

    // Обработки кнопки MID
    if (BTN_Handler(BTN_M) == true) {

      // Если задача запушена...
      if (eTaskGetState(id_task_led_track) != eSuspended) {

        // Остановка задачи
        vTaskSuspend(id_task_led_track);

        // Если мьютекс не захвачен задачей отслеживания...
        if (xSemaphoreGetMutexHolder(id_semaphore_led) != id_task_led_track) {

          // Захват мьютекса
          xSemaphoreTake(id_semaphore_led, portMAX_DELAY);
        }
      }

      // Иначе - если задача остановлена...
      else {

        // Запуск задачи
        vTaskResume(id_task_led_track);

        // Если мьютекст захвачен текущей задачей...
        if (xSemaphoreGetMutexHolder(id_semaphore_led) == id_task_led_pause) {

          // Освобождение мьютекса
          xSemaphoreGive(id_semaphore_led);
        }
      }
    }

    // Задержка перед началом следующей обработки
    vTaskDelay(100);
  }
}

// Функция обратного вызова таймера
void CallbackLedBlink(TimerHandle_t id_timer)
{
  // Захват мьютекса (если он свободен)
  if (xSemaphoreTake(id_semaphore_led, 0) == pdPASS) {

    // Состояние светодиодов
    static uint8_t led_num = 0;

    // Переключение светодиода
    MDR_PORTA->RXTX ^= (1 << led_num);

    // Переход к следующему состоянию
    led_num = (led_num + 1) % 8;

    // Сохранение состояния порта
    uint8_t led_state = MDR_PORTA->RXTX & 0xFF;

    // Передача сообщения в конец очереди
    xQueueSendToBack(id_queue_led, &led_state, 0);

    // Освобождение мьютекса
    xSemaphoreGive(id_semaphore_led);
  }
}
