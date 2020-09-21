/******************************************************************************
 * @File: led.h
 * @Author: Milandr, L.
 * @Project: #05.1_DMA_[Software_Request]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 30.04.2020
 * @Purpose: Конфигурация портов для работы светодиодов
 ******************************************************************************/

// Условная компиляция
#ifndef __LED_H
#define __LED_H

// Подключение заголовков
#include "link.h"

// Прототипы функций
void LED_Init(void);
void LED_SwitchState(void);

#endif
