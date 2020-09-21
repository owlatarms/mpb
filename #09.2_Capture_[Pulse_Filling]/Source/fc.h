/******************************************************************************
 * @File: fc.h
 * @Author: Milandr, L.
 * @Project: #09.2_Capture_[Pulse_Filling]
 * @Version: 1.0.0
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 24.07.2020
 * @Purpose: Реализация частотомера с алгоритмом импульсного заполнения
 ******************************************************************************/

// Условная компиляция
#ifndef __FC_H
#define __FC_H

// Подключение заголовков
#include "link.h"

// Константы частотомера
#define FC_FRQ     10000000  // Частота заполнения (в Гц)
#define FC_SRM     32        // Размер выборки сигнала
#define FC_TIMEOUT 1000      // Тайм-аут измерений (в миллисекундах)

// Делитель частоты
#define TIMER_PSG (SystemCoreClock / FC_FRQ)

// Псевдонимы состояний конечного автомата
typedef enum {
  FC_FSM_RDY = 0,  // «Измеритель готов»
  FC_FSM_BSY = 1   // «Измеритель занят»
} fc_fsm_t;

// Кроссмодульные переменные
extern volatile uint32_t fc_sample[FC_SRM];
extern volatile bool flg_fc_eoc;

// Прототипы функций
void TIMER_Reset(MDR_TIMER_TypeDef *MDR_TIMERx);
void FC_Init(void);
void FC_Count(uint32_t *frequency, double *epsilon);

#endif
