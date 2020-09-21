/******************************************************************************
 * @File: dac.h
 * @Author: Milandr, L.
 * @Project: #06.2_DAC_[Cyclic_Conversion]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 19.05.2020
 * @Purpose: Конфигурация цифро-аналогового преобразователя
 ******************************************************************************/

// Условная компиляция
#ifndef __DAC_H
#define __DAC_H

// Подключение заголовков
#include "link.h"

// Псевдонимы форм сигналов
typedef enum {
  SINE     = 0,  // Синусоидальный
  SQUARE   = 1,  // Прямоугольный
  SAWTOOTH = 2,  // Пилообразный
  TRIANGLE = 3,  // Треугольный
  NOISE    = 4   // Шумоподобный
} waveform_t;

// Структура параметров сигнала
typedef struct
{
  double constant;            // Константа (в вольтах)
  double amplitude;           // Амплитуда (в вольтах)
  uint32_t frequency;         // Частота (в герцах)
  waveform_t waveform;        // Форма сигнала
  volatile uint16_t *buffer;  // Буфер хранения
} signal_t;

// Число Пи
#define PI 3.1415926536F

// Константы ЦАП
#define DAC_REF 3.30F  // Опорное напряжение (в вольтах)
#define DAC_MAX 4095   // Разрешение
#define DAC_SRM 128    // Коэф. дискретизации

// Макрос вычисления кода для требуемого напряжения
#define VLT_TO_DAC(VAL) (uint32_t)((VAL) * DAC_MAX / DAC_REF)

// Кроссмодульные переменные
extern volatile uint16_t dac_sample[DAC_SRM];

// Прототипы функций
void DAC_Reset(void);
void DAC_Init(void);
void DAC_GenerateWaveform(signal_t *signal);
void DAC_AssertWaveform(signal_t *signal);
void DAC_PrintBrief(signal_t *signal);

#endif
