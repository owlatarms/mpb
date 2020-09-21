/******************************************************************************
 * @File: main.c
 * @Author: Milandr, L.
 * @Project: #07.3_ADC_[Multichannel_Conversion]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 27.03.2020
 * @Purpose: Основной модуль
 * @Description:
 * В данном проекте выполняются аналого-цифровые преобразования в режиме
 * последовательного переключения каналов. В качестве каналов выбраны каскады
 * измерения токов потребления. Результаты отображаются на дисплей.
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

  // Инициализация АЦП
  ADC_Init();

  // Инициализация периферии
  PRPH_Init();

  // Инициализация переменных
  uint32_t *adc_result = 0;  // Указатель на массив результатов
  uint32_t amper_main = 0;   // Ток основной платы
  uint32_t amper_heat = 0;   // Ток нагревателя

  // Запуск цикла преобразований
  MDR_ADC->ADC1_CFG |= ADC1_CFG_REG_ADON;

  // Основной цикл
  while (true) {

    // Блок работы с АЦП
    if (Wait(&mark[0], 100) == true) {

      // Опрос флага завершения цикла преобразований
      if (flg_adc_eoc == true) {

        // Сброс флага
        flg_adc_eoc = false;

        // Вычисление среднего арифметического выборки
        adc_result = ADC_GetAverage(adc_sample);

        // Вычисление токов (в миллиамперах)
        amper_main = ADC_TO_AMP(AMP_K_MAIN, adc_result[5]);  // Ток основной платы
        amper_heat = ADC_TO_AMP(AMP_K_HEAT, adc_result[6]);  // Ток нагревателя

        // Отображение результатов
        LCD_PrintString("    Im = %u mA", 3, amper_main);    // Ток основной платы
        LCD_PrintString("    Ih = %u mA", 5, amper_heat);    // Ток нагревателя

        // Запуск цикла преобразований
        MDR_ADC->ADC1_CFG |= ADC1_CFG_REG_ADON;
      }
    }

    // Блок работы с периферией
    if (Wait(&mark[1], 100) == true) {

      // Нагреватель
      if (BTN_Handler(BTN_U) == true) {     // Обработка кнопки UP
        MDR_PORTE->RXTX ^= PORT_RXTX0_Msk;  // Переключение состояния нагревателя
      }

      // Подсветка дисплея
      if (BTN_Handler(BTN_M) == true) {     // Обработка кнопки MID
        MDR_PORTE->RXTX ^= PORT_RXTX2_Msk;  // Переключение состояния подсветки дисплея
      }

      // Подсветка платы
      if (BTN_Handler(BTN_D) == true) {     // Обработка кнопки DOWN
        MDR_PORTC->RXTX ^= PORT_RXTX2_Msk;  // Переключение состояния подсветки платы
      }
    }
  }
}
