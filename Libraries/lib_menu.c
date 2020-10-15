/******************************************************************************
 * @File: lib_menu.c
 * @Author: Milandr, L.
 * @Project: Library
 * @Version: 1.1.0
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 15.10.2020
 * @Purpose: Библиотека реализации меню
 ******************************************************************************/

// Подключение заголовка
#include "lib_menu.h"

// Глобальные переменные
// Пустой (несуществующий) элемент меню
menu_t m_null = {NULL, NULL, NULL, NULL, NULL, NULL};

// Текущий элемент меню
menu_t* m_curr = &m_null;

// Инициализация меню
void MENU_Init(menu_t *item)
{
  // Установка исходного элемента меню
  m_curr = item;

  // Отображение исходной страницы
  MENU_PrintPage(m_curr);

  // Отображение указателя
  MENU_PrintPtr(m_curr);
}

// Перемещение по меню
void MENU_Move(menu_t* item)
{
  // Если выбранный элемент существует...
  if (item != &m_null) {

    // Отображение новой страницы меню,
    // если выбранный и текущий элементы расположены на разных страницах
    if (m_curr->page != item->page) {
      MENU_PrintPage(item);
    }

    // Отображение указателя
    MENU_PrintPtr(item);

    // Запись выбранного элемента в текущий
    m_curr = item;
  }
}

// Выполнение функции элемента
void MENU_Exec(menu_t* item)
{
  // Выполнение функции элемента,
  // если она существует
  if (item->func != NULL) {
    item->func();
  }
}

// Отображение страницы меню
void MENU_PrintPage(menu_t* item)
{
  // Поиск начального элемента страницы
  while (item->pos != 0) {item = item->previous;}

  // Цикл отображения элементов страницы
  for (int i = 0; i < MENU_ITEM_LIMIT; i++) {

    // Отображение элемента
    LCD_PrintString(item->text, i + MENU_STR_POS, item->data);

    // Переход к следующему элементу
    if (item != &m_null) {
      item = item->next;
    }
  }
}

// Отображение указателя
void MENU_PrintPtr(menu_t* item)
{
  // Стирание столбца указателей
  for (int i = 0; i < MENU_ITEM_LIMIT; i++) {
    LCD_PrintSymbol(' ', MENU_PTR_POS, (MENU_STR_POS + i));
  }

  // Отображение указателя на требуемую позицию
  LCD_PrintSymbol('#', MENU_PTR_POS, (MENU_STR_POS + item->pos));
}
