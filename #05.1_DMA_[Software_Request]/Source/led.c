/******************************************************************************
 * @File: led.c
 * @Author: Milandr, L.
 * @Project: #05.1_DMA_[Software_Request]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 30.04.2020
 * @Purpose: Конфигурация портов для работы светодиодов
 ******************************************************************************/

// Подключение заголовка
#include "led.h"

// Инициализация системы для работы светодиодов
void LED_Init(void)
{
  // Включение тактирования порта A
  MDR_RST_CLK->PER_CLOCK |= RST_CLK_PCLK_PORTA_Msk;

  // Конфигурация линий PA0...PA7
  MDR_PORTA->OE     |=  0x000000FF;  // Направление (вывод)
  MDR_PORTA->FUNC   &= ~0x0000FFFF;  // Функция (ввод-вывод)
  MDR_PORTA->ANALOG |=  0x000000FF;  // Режим (цифровой)
  MDR_PORTA->PULL   &= ~0x00FF00FF;  // Подтяжка (отключена)
  MDR_PORTA->PD     &= ~0x00FF00FF;  // Управление (драйвер)
  MDR_PORTA->PWR    |=  0x0000FFFF;  // Крутизна (высокая)
  MDR_PORTA->GFEN   &= ~0x000000FF;  // Фильтр (не используется)

  // Исходное отключение светодиодов
  MDR_PORTA->RXTX &= ~0x000000FF;
}

// Реализация состояний светодиодов
void LED_SwitchState(void)
{
  // Состояние светодиодов
  static uint8_t led_num = 0;

  // Отключение всех светодиодов (PA0...PA3)
  MDR_PORTA->RXTX &= ~0x000000FF;

  // Включение целевого светодиода
  MDR_PORTA->RXTX ^= (1 << led_num);

  // Переход к следующему состоянию
  led_num  = (led_num + 1) % 8;
}
