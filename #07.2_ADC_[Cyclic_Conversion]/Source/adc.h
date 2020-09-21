/******************************************************************************
 * @File: adc.h
 * @Author: Milandr, L.
 * @Project: #07.2_ADC_[Cyclic_Conversion]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 18.03.2020
 * @Purpose: Управление аналого-цифровым преобразователем
 ******************************************************************************/

// Условная компиляция
#ifndef __ADC_H
#define __ADC_H

// Подключение заголовков
#include "link.h"

// Константы АЦП
#define ADC_SRM 256    // Размер выборки
#define ADC_REF 3.30F  // Опорное напряжение (в вольтах)
#define ADC_MAX 4095   // Разрешение АЦП

// Константы температуры
#define V_STP 0.0195F  // Цена одного градуса (в вольтах)
#define V_OFS 0.40F    // Смещение относительно нуля (в вольтах)

// Макрос вычисления температуры (в градусах Цельсия)
#define ADC_TO_TMP(VAL) ((VAL) * ADC_REF / ADC_MAX - V_OFS) / V_STP

// Кроссмодульные переменные
extern volatile uint32_t adc_sample[ADC_SRM];
extern volatile bool flg_adc_eoc;

// Прототипы функций
void ADC_Reset(void);
void ADC_Init(void);
uint32_t ADC_GetAverage(volatile uint32_t *buffer);

#endif
