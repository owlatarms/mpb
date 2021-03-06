﻿/******************************************************************************
 * @File: main.c
 * @Author: Milandr, L.
 * @Project: #08.1_PWM_[Variable_Duty_Cycle]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 26.03.2020
 * @Purpose: Основной модуль
 * @Description:
 * В данном проекте реализовано управление яркостью подсветки платы посредством
 * широтно-импульсной модуляции. Модуляция организована на таймере 3 (канал 1).
 * Скважность импульсов (S) задаётся положением ручки потенциометра через АЦП.
 * В данном контексте под скважностью понимается фактически коэффициент заполнения.
 ******************************************************************************/

// Подключение заголовка
#include "main.h"

// Основная функция
int main(void)
{
  // Общая инициализация
  CPU_Init();   // Система тактирования
  TICK_Init();  // Системный таймер
  LCD_Init();   // ЖК-дисплей

  // Инициализация АЦП
  ADC_Init();

  // Инициализация ШИМ
  PWM_Init();

  // Инициализация переменных
  uint32_t *adc_result = 0;  // Указатель на массив результатов
  uint32_t power_cycle = 0;  // Расчётная скважность

  // Запуск цикла преобразований
  MDR_ADC->ADC1_CFG |= ADC1_CFG_REG_ADON;

  // Основной цикл
  while (true) {

    // Блок работы с АЦП
    if ((Wait(&mark[0], 100) == true)) {

      // Опрос флага завершения цикла преобразований
      if (flg_adc_eoc == true) {

        // Сброс флага
        flg_adc_eoc = false;

        // Медианная фильтрация выборки
        adc_result = ADC_MedianFilter(adc_sample);

        // Вычисление скважности (в процентах)
        power_cycle = ADC_TO_PRC(adc_result[4]);

        // Установка скважности
        PWM_SetPowerCycle(MDR_TIMER3, TIMER_CHANNEL1, power_cycle);

        // Отображение результатов
        LCD_PrintString("     S = %u%%", 3, power_cycle);    // Скважность
        LCD_PrintString("     D = %u",   5, adc_result[4]);  // Цифровое значение

        // Запуск цикла преобразований
        MDR_ADC->ADC1_CFG |= ADC1_CFG_REG_ADON;
      }
    }
  }
}
