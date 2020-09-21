/******************************************************************************
 * @File: pwm.h
 * @Author: Milandr, L.
 * @Project: #08.1_PWM_[Variable_Duty_Cycle]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 26.03.2020
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

// Частота ШИМ (в герцах)
#define PWM_F 10000

// Прототипы функций
void TIMER_Reset(MDR_TIMER_TypeDef *MDR_TIMERx);
void PWM_Init(void);
void PWM_SetFrequency(MDR_TIMER_TypeDef *MDR_TIMERx, uint32_t frequency);
void PWM_SetPowerCycle(MDR_TIMER_TypeDef *MDR_TIMERx, timer_channel_t channel, uint32_t power_cycle);

#endif
