/******************************************************************************
 * @File: tmr.h
 * @Author: Milandr, L.
 * @Project: #05.2_DMA_[Hardware_Request]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 07.05.2020
 * @Purpose: Конфигурация аппаратных таймеров
 ******************************************************************************/

// Условная компиляция
#ifndef __TMR_H
#define __TMR_H

// Подключение заголовков
#include "link.h"

// Прототипы функций
void TIMER_Reset(MDR_TIMER_TypeDef *MDR_TIMERx);
void TIMER_Init(void);

#endif
