/******************************************************************************
 * @File: wdg.h
 * @Author: Milandr, L.
 * @Project: #03.3_Timer_[Watchdog]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 08.04.2020
 * @Purpose: Конфигурация сторожевого таймера
 ******************************************************************************/

// Условная компиляция
#ifndef __WDG_H
#define __WDG_H

// Подключение заголовков
#include "link.h"

// Тактовая частота генератора LSI (в герцах)
#define LSI_CLK 30000

// Константы сторожевого таймера
#define IWDG_RES     12                     // Разрядность
#define IWDG_PR_MAX  6                      // Допуск делителя
#define IWDG_RLR_MAX ((1 << IWDG_RES) - 1)  // Допуск счётчика

// Период работы сторожевого таймера (в миллисекундах)
#define IWDG_RLD_TIME 5000

// Ключевые слова сторожевого таймера
#define IWDG_KR_UNLOCK 0x5555  // Разрешение модификации регистров PR и RLR
#define IWDG_KR_RELOAD 0xAAAA  // Перезагрузка таймера
#define IWDG_KR_LAUNCH 0xCCCC  // Запуск таймера

// Прототипы функций
void LSI_Init(void);
void IWDG_Init(void);

#endif
