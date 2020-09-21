﻿/******************************************************************************
 * @File: main.c
 * @Author: Milandr, L.
 * @Project: #02.3_GPIO_[LCD&Menu]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 12.04.2020
 * @Purpose: Основной модуль
 * @Description:
 * В данном проекте реализовано двухуровневое древовидное меню управления.
 * Навигация осуществляется кнопками UP, DOWN и MID. Использованный программный
 * подход позволяет гибко и тривиально создавать меню разной степени вложенности.
 ******************************************************************************/

// Подключение заголовка
#include "main.h"

// Основная функция
int main(void)
{
  // Общая инициализация
  BTN_Init();  // Кнопки
  LCD_Init();  // ЖК-дисплей

  // Инициализация светодиодов
  LED_Init();

  // Инициализация меню
  MENU_Init();

  // Основной цикл
  while (true) {

    // Обработка кнопки UP
    if (BTN_Handler(BTN_U) != false) {
      MENU_Move(m_curr->previous);      // Переход к предыдущему элементу меню
    }

    // Обработка кнопки DOWN
    if (BTN_Handler(BTN_D) != false) {
      MENU_Move(m_curr->next);          // Переход к следующему элементу меню
    }

    // Обработка кнопки MID
    if (BTN_Handler(BTN_M) != false) {
      MENU_Exec(m_curr);                // Выполнение функции элемента (если она существует)
      MENU_Move(m_curr->select);        // Переход к подменю
    }
  }
}
