/******************************************************************************
 * @File: bkp.h
 * @Author: Milandr, L.
 * @Project: #04.2_Power_Domain_[Sleep&Backup]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 29.04.2020
 * @Purpose: Конфигурация батарейного домена
 ******************************************************************************/

// Условная компиляция
#ifndef __BKP_H
#define __BKP_H

// Подключение заголовков
#include "link.h"

// Макрос ожидания завершения работы с регистрами RTC
// (данная процедура должна выполняться перед каждым взаимодействием с регистрами RTC)
#define RTC_WAIT_FOR_UPDATE for (int i = 0; ((MDR_BKP->RTC_CS & BKP_RTC_CS_WEC) != false) && (i < 1000); i++);

// Установка периода сигнализации RTC (в секундах)
#define RTC_SET_ALARM(SEC) RTC_WAIT_FOR_UPDATE;                          \
                           MDR_BKP->RTC_ALRM = MDR_BKP->RTC_CNT + (SEC)

// Прототипы функций
void IRQ_Mask(uint8_t priority);
void CPU_SleepCfg(void);
void CPU_PutToSleep(void);
void RTC_Init(void);
void PVD_Init(void);


#endif
