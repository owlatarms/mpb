/******************************************************************************
 * @File: tmr.h
 * @Author: Milandr, L.
 * @Project: #03.1_Timer_[General_Purpose]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 06.04.2020
 * @Purpose: Конфигурация аппаратных таймеров
 ******************************************************************************/

// Условная компиляция
#ifndef __TMR_H
#define __TMR_H

// Подключение заголовков
#include "link.h"

// Кроссмодульные переменные
extern volatile bool flg_tmr_eoc;
extern volatile uint32_t tmr_rld_cnt;

// Прототипы функций
void TIMER_Reset(MDR_TIMER_TypeDef *MDR_TIMERx);
void TIMER1_Init(void);
void TIMER2_Init(void);

#endif
