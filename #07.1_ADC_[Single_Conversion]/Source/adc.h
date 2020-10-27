/******************************************************************************
 * @File: adc.h
 * @Author: Milandr, L.
 * @Project: #07.1_ADC_[Single_Conversion]
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
#define ADC_REF 3.30F  // Опорное напряжение (в вольтах)
#define ADC_MAX 4095   // Разрешение АЦП

// Макрос вычисления напряжения
#define ADC_TO_VLT(VAL) (ADC_REF * (VAL) / ADC_MAX)

// Кроссмодульные переменные
extern volatile uint16_t adc_result;
extern volatile bool flg_adc_eoc;

// Прототипы функций
void ADC_Reset(void);
void ADC_Init(void);

#endif
