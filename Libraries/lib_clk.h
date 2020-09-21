/******************************************************************************
 * @File: lib_clk.h
 * @Author: Milandr, L.
 * @Project: Library
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 12.03.2020
 * @Purpose: Конфигурация системы тактирования микроконтроллера
 ******************************************************************************/

// Условная компиляция
#ifndef __LIB_CLK_H
#define __LIB_CLK_H

// Подключение библиотеки микроконтроллера
#include <1986VE9x.h>

// Кроссмодульные переменные
extern uint32_t SystemCoreClock;

// Прототипы функций
void CPU_Reset(void);
void CPU_Init(void);

#endif
