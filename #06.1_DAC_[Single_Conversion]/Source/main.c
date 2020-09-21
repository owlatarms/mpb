﻿/******************************************************************************
 * @File: main.c
 * @Author: Milandr, L.
 * @Project: #06.1_DAC_[Single_Conversion]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 15.05.2020
 * @Purpose: Основной модуль
 * @Description:
 * В данном проекте реализованы однократные цифро-аналоговые преобразования.
 * Изменение уровня напряжения ЦАП осуществляется кнопками. Расчётное напряжение
 * и цифровой код отображаются на дисплей.
 ******************************************************************************/

// Подключение заголовка
#include "main.h"

// Основная функция
int main(void)
{
  // Общая инициализация
  CPU_Init();   // Система тактирования
  TICK_Init();  // Системный таймер
  BTN_Init();   // Кнопки
  LCD_Init();   // ЖК-дисплей

  // Инициализация ЦАП
  DAC_Init();

  // Напряжение ЦАП
  float voltage = 1.20;

  // Установка исходного напряжения ЦАП
  DAC_SetVoltage(&voltage);

  // Основной цикл
  while (true) {

    // Блок работы с кнопкой
    if (Wait(&mark[0], 100) == true) {

      // Обработка кнопки UP
      if (BTN_Handler(BTN_U) == true) {

        // Увеличение напряжения
        voltage += DAC_STP;

        // Установка напряжения ЦАП
        DAC_SetVoltage(&voltage);
      }

      // Обработка кнопки DOWN
      if (BTN_Handler(BTN_D) == true) {

        // Уменьшение напряжения
        voltage -= DAC_STP;

        // Установка напряжения ЦАП
        DAC_SetVoltage(&voltage);
      }
    }
  }
}
