/******************************************************************************
 * @File: led.c
 * @Author: Milandr, L.
 * @Project: #10.3_RTOS_[Timer&Queue&Mutex]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 22.06.2020
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
