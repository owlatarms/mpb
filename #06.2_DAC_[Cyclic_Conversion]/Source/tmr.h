/******************************************************************************
 * @File: tmr.h
 * @Author: Milandr, L.
 * @Project: #06.2_DAC_[Cyclic_Conversion]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 19.05.2020
 * @Purpose: Конфигурация аппаратных таймеров
 ******************************************************************************/

// Условная компиляция
#ifndef __TMR_H
#define __TMR_H

// Подключение заголовков
#include "link.h"

// Константы таймера
#define TIMER_ARR_MAX 65535  // Разрешение счётчика
#define TIMER_ARR_MIN 9      // Мин. значение счётчика
#define TIMER_PSG     1      // Делитель частоты

// Макрос вычисления значения перезагрузки
// для генерации сигнала с заданной частотой (F) и коэф. дискретизации (M)
#define FRQ_TO_CNT(F, M) (uint16_t)(SystemCoreClock / ((F) * (M) * TIMER_PSG) - 1)

// Прототипы функций
void TIMER_Reset(MDR_TIMER_TypeDef *MDR_TIMERx);
void TIMER_Init(void);

#endif
