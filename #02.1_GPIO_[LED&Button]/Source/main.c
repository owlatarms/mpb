/******************************************************************************
 * @File: main.c
 * @Author: Milandr, L.
 * @Project: #02.1_GPIO_[LED&Button]
 * @Version: 1.0.2
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 14.10.2020
 * @Purpose: Основной модуль
 * В данном проекте реализовано управление светодиодами с использованием кнопки.
 * В любой момент времени горит только один светодиод. Каждое нажатие кнопки
 * гасит текущий светодиод и зажигает следующий.
 ******************************************************************************/

// Подключение заголовка
#include "main.h"

// Основная функция
int main(void)
{
  // Инициализация кнопок
  BTN_Init();

  // Инициализация светодиодов
  LED_Init();

  // Основной цикл
  while (true) {

    // Обработка кнопки MID
    if (BTN_Handler(BTN_M) == true) {

      // Переключение светодиодов
      LED_SwitchState();
    }
  }
}
