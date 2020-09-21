/******************************************************************************
 * @File: fc.h
 * @Author: Milandr, L.
 * @Project: #09.1_Capture_[Discrete_Count]
 * @Version: 1.0.0
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 20.07.2020
 * @Purpose: Реализация частотомера с алгоритмом дискретного счёта
 ******************************************************************************/

// Условная компиляция
#ifndef __FC_H
#define __FC_H

// Подключение заголовков
#include "link.h"

// Период измерения частоты (в cекундах)
#define FC_GATE_TIME 0.5F

// Делитель частоты таймера
#define TIMER_PSG 4096

// Макрос вычисления значения перезагрузки по заданному периоду
#define TIME_TO_CNT(T) (uint16_t)((T) * SystemCoreClock / TIMER_PSG - 1)

// Псевдонимы состояний конечного автомата
typedef enum {
  FC_FSM_RDY = 0,  // «Измеритель готов»
  FC_FSM_BSY = 1   // «Измеритель занят»
} fc_fsm_t;

// Прототипы функций
void TIMER_Reset(MDR_TIMER_TypeDef *MDR_TIMERx);
void FC_Init(void);
void FC_Count(uint32_t *frequency, double *epsilon);

#endif
