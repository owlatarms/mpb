/******************************************************************************
 * @File: lib_tick.h
 * @Author: Milandr, L.
 * @Project: Library
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 20.03.2020
 * @Purpose: Реализация временной базы
 ******************************************************************************/

// Условная компиляция
#ifndef __LIB_TICK_H
#define __LIB_TICK_H

// Подключение библиотеки микроконтроллера
#include <1986VE9x.h>

// Подключение конфигурации системы тактирования
#include <lib_clk.h>

// Частота системных квантов (в Гц)
#define SYSTEM_TICK_RATE 1000

// Кол-во независимых интервалов ожидания
#define WAIT_NUM 16

// Кроссмодульные переменные
extern uint32_t system_ticks;
extern uint32_t mark[WAIT_NUM];

// Прототипы функций
void TICK_Init(void);
bool Wait(uint32_t *ticks_mark, uint32_t wait_ticks);

#endif
