/******************************************************************************
 * @File: adc.h
 * @Author: Milandr, L.
 * @Project: #08.1_PWM_[Variable_Duty_Cycle]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 27.03.2020
 * @Purpose: Управление аналого-цифровым преобразователем
 ******************************************************************************/

// Условная компиляция
#ifndef __ADC_H
#define __ADC_H

// Подключение заголовков
#include "link.h"

// Константы АЦП
#define ADC_CH_NUM 8      // Количество каналов АЦП
#define ADC_SRM    256    // Размер выборки
#define ADC_MAX    4095   // Разрешение АЦП

// Макс. скважность (в процентах)
#define S_MAX 100

// Макрос вычисления скважности
#define ADC_TO_PRC(VAL) S_MAX * (VAL) / ADC_MAX

// Кроссмодульные переменные
extern volatile uint32_t adc_sample[ADC_SRM];
extern volatile bool flg_adc_eoc;

// Прототипы функций
void ADC_Reset(void);
void ADC_Init(void);
uint32_t *ADC_MedianFilter(volatile uint32_t *buffer);

#endif
