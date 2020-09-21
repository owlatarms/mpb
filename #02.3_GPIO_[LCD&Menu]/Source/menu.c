/******************************************************************************
 * @File: menu.c
 * @Author: Milandr, L.
 * @Project: #02.3_GPIO_[LCD&Menu]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 12.04.2020
 * @Purpose: Реализация меню
 ******************************************************************************/

// Подключение заголовка
#include "menu.h"

// Глобальные переменные
// Пустой (несуществующий) элемент меню
menu_t m_null = {NULL, NULL, NULL, NULL, NULL, NULL};

// Текущий элемент меню
menu_t* m_curr = &m_null;

// Буфер данных элементов меню
char m_data[LED_NUM][2] = {NULL};

// Создание элементов меню
// Основная страница
MENU_ADD(m_p0i0, m_p0i1, m_null, m_p1i0, NULL,        NULL,      "    LED 0...2");
MENU_ADD(m_p0i1, m_p0i2, m_p0i0, m_p2i0, NULL,        NULL,      "    LED 3...5");
MENU_ADD(m_p0i2, m_null, m_p0i1, m_null, LED_Reset,   NULL,      "    Reset");

// Страница LED 0...2
MENU_ADD(m_p1i0, m_p1i1, m_null, m_null, LED_Handler, m_data[0], "    LED 0: %s");
MENU_ADD(m_p1i1, m_p1i2, m_p1i0, m_null, LED_Handler, m_data[1], "    LED 1: %s");
MENU_ADD(m_p1i2, m_p1i3, m_p1i1, m_null, LED_Handler, m_data[2], "    LED 2: %s");
MENU_ADD(m_p1i3, m_null, m_p1i2, m_p0i0, NULL,        NULL,      "    Back");

// Страница LED 3...5
MENU_ADD(m_p2i0, m_p2i1, m_null, m_null, LED_Handler, m_data[3], "    LED 3: %s");
MENU_ADD(m_p2i1, m_p2i2, m_p2i0, m_null, LED_Handler, m_data[4], "    LED 4: %s");
MENU_ADD(m_p2i2, m_p2i3, m_p2i1, m_null, LED_Handler, m_data[5], "    LED 5: %s");
MENU_ADD(m_p2i3, m_null, m_p2i2, m_p0i1, NULL,        NULL,      "    Back");

// Инициализация меню
void MENU_Init(void)
{
  // Установка исходного элемента меню
  m_curr = &m_p0i0;

  // Отображение исходной страницы
  MENU_PrintPage(m_curr);

  // Отображение указателя
  MENU_PrintPtr(m_curr);
}

// Перемещение по меню
void MENU_Move(menu_t* item)
{
  // Определение позиции выбранного элемента
  uint8_t item_pos = 0;
  for (menu_t *temp = item;
      (temp != &m_null) && (temp->previous != &m_null);
       temp = temp->previous, item_pos++);

  // Если выбранный элемент существует,
  // и расположен на допустимой позиции...
  if (item != &m_null && item_pos < MENU_ITEM_LIMIT) {

    // Отображение новой страницы меню,
    // если выбранный и текущий элементы не соседние
    if ((item != m_curr->next) && (item != m_curr->previous)) {
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
  for (uint8_t i = 0;
      (item->previous != &m_null) && (i < MENU_ITEM_LIMIT);
       item = item->previous, i++);

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
  for (uint8_t i = 0; i < MENU_ITEM_LIMIT; i++) {
    LCD_PrintSymbol(' ', MENU_PTR_POS, (MENU_STR_POS + i));
  }

  // Определение позиции выбранного элемента
  uint8_t item_pos = 0;
  for (menu_t *temp = item;
      (temp != &m_null) && (temp->previous != &m_null);
       temp = temp->previous, item_pos++);

  // Отображение указателя требуемую позицию
  LCD_PrintSymbol('#', MENU_PTR_POS, (MENU_STR_POS + item_pos));
}
