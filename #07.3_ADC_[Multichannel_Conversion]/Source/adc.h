/******************************************************************************
 * @File: adc.h
 * @Author: Milandr, L.
 * @Project: #07.3_ADC_[Multichannel_Conversion]
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
#define ADC_REF    3.30F  // Опорное напряжение (в вольтах)
#define ADC_MAX    4095   // Разрешение АЦП

// Константы измерения токов
#define AMP_R_SHUNT     0.1F   // Сопротивление шунта (в омах)
#define AMP_K_MAIN      200    // КУ основной платы
#define AMP_K_HEAT      100    // КУ нагревателя
#define AMP_K_LED       100    // КУ подсветки

// Макрос вычисления тока (в миллиамперах)
#define ADC_TO_AMP(AMP_K, VAL) (uint32_t)(((VAL) * ADC_REF * 1000) / (ADC_MAX * AMP_K * AMP_R_SHUNT))

// Кроссмодульные переменные
extern volatile uint32_t adc_sample[ADC_SRM];
extern volatile bool flg_adc_eoc;

// Прототипы функций
void ADC_Reset(void);
void ADC_Init(void);
uint32_t *ADC_GetAverage(volatile uint32_t *buffer);

#endif
