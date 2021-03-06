/*******************************************************************************
 * @File: lib_btn.h
 * @Author: Milandr, L.
 * @Project: Library
 * @Version: 1.2.0
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 24.03.2020
 * @Purpose: Библиотека для работы кнопок
 *******************************************************************************/

// Условная компиляция
#ifndef __LIB_BTN_H
#define __LIB_BTN_H

// Подключение библиотеки микроконтроллера
#include <1986VE9x.h>

// Количество кнопок
#define BTN_NUM 6

// Предел счётчика
#define BTN_CNT_LIM 128

// Псевдонимы кнопок
typedef enum {
  BTN_U = 0,  // Кнопка UP
  BTN_D = 1,  // Кнопка DOWN
  BTN_L = 2,  // Кнопка LEFT
  BTN_R = 3,  // Кнопка RIGHT
  BTN_M = 4,  // Кнопка MID
  BTN_S = 5   // Кнопка SENSE
} btn_alias_t;

// Псевдонимы машины состояний
// (Finite-State Machine, FSM)
typedef enum {
  BTN_FSM_PRESS   = 0,  // «Ожидание нажатия»
  BTN_FSM_RELEASE = 1,  // «Ожидание отпускания»
} btn_fsm_t;

// Прототипы функций
void BTN_Init(void);
bool BTN_Handler(btn_alias_t alias);

#endif
