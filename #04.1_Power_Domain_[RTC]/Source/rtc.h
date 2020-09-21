/******************************************************************************
 * @File: rtc.c
 * @Author: Milandr, L.
 * @Project: #04.1_Power_Domain_[RTC]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 28.04.2020
 * @Purpose: Конфигурация часов реального времени
 ******************************************************************************/

// Условная компиляция
#ifndef __RTC_H
#define __RTC_H

// Подключение заголовков
#include "link.h"

// Макрос ожидания завершения работы с регистрами RTC
// (данная процедура должна выполняться перед каждым взаимодействием с регистрами RTC)
#define RTC_WAIT_FOR_UPDATE for (int i = 0; ((MDR_BKP->RTC_CS & BKP_RTC_CS_WEC) != false) && (i < 1000); i++);

// Прототипы функций
void RTC_Init(void);
void RTC_SetTime(void);
void RTC_PrintTime(uint32_t unix_time);

#endif
