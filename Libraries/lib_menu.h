/******************************************************************************
 * @File: lib_menu.h
 * @Author: Milandr, L.
 * @Project: Library
 * @Version: 1.1.0
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 15.10.2020
 * @Purpose: Библиотека реализации меню
 ******************************************************************************/

// Условная компиляция
#ifndef __LIB_MENU_H
#define __LIB_MENU_H

// Подключение стандартных библиотек
#include <stdio.h>
#include <1986VE9x.h>
#include <lib_lcd.h>

// Типы данных
// Указатель на функцию
typedef void (*fptr_t)(void);

// Структура элементов меню
typedef struct {
  void       *next;      // Следующий элемент
  void       *previous;  // Предыдущий элемент
  void       *select;    // Подменю или действие
  fptr_t      func;      // Функция элемента
  uint8_t     page;      // Страница элемента
  uint8_t     pos;       // Позиция элемента
  char       *data;      // Данные элемента
  const char *text;      // Текст элемента
} menu_t;

// Кол-во элементов на странице
#define MENU_ITEM_LIMIT 4

// Константы отображения
#define MENU_STR_POS 3  // Позиция первой строки меню (ординат)
#define MENU_PTR_POS 2  // Позиция указателя (абсцисс)

// Макрос добавления элемента меню
#define MENU_ADD(name, next, previous, select, func, page, pos, data, text)     \
  extern menu_t next;                                                           \
  extern menu_t previous;                                                       \
  extern menu_t select;                                                         \
  menu_t name = {(void*)&next,  (void*)&previous, (void*)&select, (fptr_t)func, \
                 (uint8_t)page, (uint8_t)pos,     (char*)data,    (char*)text};

// Кроссмодульные переменные
extern menu_t* m_curr;

// Прототипы функций
void MENU_Init(menu_t *item);
void MENU_Move(menu_t* item);
void MENU_Exec(menu_t* item);
void MENU_PrintPage(menu_t* item);
void MENU_PrintPtr(menu_t* item);

#endif
