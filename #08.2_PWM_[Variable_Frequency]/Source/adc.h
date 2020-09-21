/******************************************************************************
 * @File: adc.h
 * @Author: Milandr, L.
 * @Project: #08.2_PWM_[Variable_Frequency]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 02.04.2020
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

// Макс. частота импульсов (в герцах)
#define FRQ_MAX 3000

// Макрос вычисления частоты (квадратичная зависимость)
#define ADC_TO_FRQ(VAL) (uint32_t)((double)FRQ_MAX / (ADC_MAX * ADC_MAX) * (VAL) * (VAL))

// Создание кроссмодульной видимости переменных
extern volatile uint32_t adc_sample[ADC_SRM];
extern volatile bool flg_adc_eoc;

// Прототипы функций
void ADC_Reset(void);
void ADC_Init(void);
uint32_t *ADC_MedianFilter(volatile uint32_t *buffer);

#endif
