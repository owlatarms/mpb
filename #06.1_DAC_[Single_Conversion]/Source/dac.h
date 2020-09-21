/******************************************************************************
 * @File: dac.h
 * @Author: Milandr, L.
 * @Project: #06.1_DAC_[Single_Conversion]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 15.05.2020
 * @Purpose: Конфигурация цифро-аналогового преобразователя
 ******************************************************************************/

// Условная компиляция
#ifndef __DAC_H
#define __DAC_H

// Подключение заголовков
#include "link.h"

// Константы ЦАП
#define DAC_REF 3.30F  // Опорное напряжение (в вольтах)
#define DAC_MAX 4095   // Разрешение
#define DAC_STP 0.50F  // Шаг изменения напряжения (в вольтах)

// Макрос вычисления кода для требуемого напряжения
#define VLT_TO_DAC(VAL) (uint32_t)((VAL) * DAC_MAX / DAC_REF)

// Прототипы функций
void DAC_Reset(void);
void DAC_Init(void);
void DAC_SetVoltage(float *voltage);

#endif
