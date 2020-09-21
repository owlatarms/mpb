/******************************************************************************
 * @File: menu.h
 * @Author: Milandr, L.
 * @Project: #02.3_GPIO_[LCD&Menu]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 12.04.2020
 * @Purpose: Реализация меню
 ******************************************************************************/

// Условная компиляция
#ifndef __MENU_H
#define __MENU_H

// Подключение заголовков модулей проекта
#include "link.h"

// Типы данных
// Указатель на функцию
typedef void (*fptr_t)(void);

// Структура элементов меню
typedef struct {
  void       *next;      // Следующий элемент
  void       *previous;  // Предыдущий элемент
  void       *select;    // Подменю или действие
  fptr_t      func;      // Функция элемента
  char       *data;      // Данные элемента
  const char *text;      // Текст элемента
} menu_t;

// Кол-во элементов на странице
#define MENU_ITEM_LIMIT 4

// Константы отображения
#define MENU_STR_POS 3  // Позиция первой строки меню (ординат)
#define MENU_PTR_POS 2  // Позиция указателя (абсцисс)

// Макрос добавления элемента меню
#define MENU_ADD(name, next, previous, select, func, data, text)  \
  extern menu_t next;                                             \
  extern menu_t previous;                                         \
  extern menu_t select;                                           \
  menu_t name = {(void*)&next, (void*)&previous, (void*)&select,  \
                 (fptr_t)func, (char*)data,      (char*)text};    \

// Кроссмодульные переменные
extern menu_t* m_curr;
extern menu_t m_p0i0, m_p0i1, m_p0i2,
              m_p1i0, m_p1i1, m_p1i2, m_p1i3,
              m_p2i0, m_p2i1, m_p2i2, m_p2i3;
extern char m_data[LED_NUM][2];

// Прототипы функций
void MENU_Init(void);
void MENU_Move(menu_t* item);
void MENU_Exec(menu_t* item);
void MENU_PrintPage(menu_t* m_item);
void MENU_PrintPtr(menu_t* item);

#endif
