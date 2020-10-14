/******************************************************************************
 * @File: led.h
 * @Author: Milandr, L.
 * @Project: #02.1_GPIO_[LED&Button]
 * @Version: 1.0.2
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 14.10.2020
 * @Purpose: Конфигурация портов для работы светодиодов
 ******************************************************************************/

// Условная компиляция
#ifndef __LED_H
#define __LED_H

// Подключение заголовков
#include "link.h"

// Кол-во используемых светодиодов
#define LED_NUM 4

// Кроссмодульные переменные
extern uint8_t led_num;

// Прототипы функций
void LED_Init(void);
void LED_SwitchState(void);

#endif
