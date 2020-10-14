/******************************************************************************
 * @File: pwm.h
 * @Author: Milandr, L.
 * @Project: #08.2_PWM_[Variable_Frequency]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 02.04.2020
 * @Purpose: Конфигурация аппаратных таймеров в режиме ШИМ
 ******************************************************************************/

// Условная компиляция
#ifndef __PWM_H
#define __PWM_H

// Подключение заголовков
#include "link.h"

// Псевдонимы каналов таймера
typedef enum {
  TIMER_CHANNEL1 = 0,
  TIMER_CHANNEL2 = 1,
  TIMER_CHANNEL3 = 2,
  TIMER_CHANNEL4 = 3
} timer_channel_t;

// Константы таймера
#define TIMER_PSG_MAX 65535  // Разрешение делителя
#define TIMER_ARR_MAX 65535  // Разрешение счётчика
#define TIMER_ARR_MIN 100    // Мин. значение счётчика

// Скважность импульсов (в процентах)
#define PWM_S 50

// Шаг двигателя (в градусах)
#define HSM_STEP 0.9F

// Прототипы функций
void TIMER_Reset(MDR_TIMER_TypeDef *MDR_TIMERx);
void PWM_Init(void);
void PWM_SetFrequency(MDR_TIMER_TypeDef *MDR_TIMERx, timer_channel_t channel, uint32_t frequency);
void PWM_SetPowerCycle(MDR_TIMER_TypeDef *MDR_TIMERx, timer_channel_t channel, uint32_t power_cycle);
void HSM_Init(void);

#endif
