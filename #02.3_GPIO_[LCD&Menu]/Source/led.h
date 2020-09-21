/******************************************************************************
 * @File: led.h
 * @Author: Milandr, L.
 * @Project: #02.3_GPIO_[LCD&Menu]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 12.04.2020
 * @Purpose: Конфигурация портов для работы светодиодов
 ******************************************************************************/

// Условная компиляция
#ifndef __LED_H
#define __LED_H

// Кол-во используемых светодиодов
#define LED_NUM 6

// Подключение заголовков
#include "link.h"

// Прототипы функций
void LED_Init(void);
void LED_Handler(void);
void LED_Reset(void);

#endif
