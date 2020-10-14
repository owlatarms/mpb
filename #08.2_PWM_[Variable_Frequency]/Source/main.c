/******************************************************************************
 * @File: main.c
 * @Author: Milandr, L.
 * @Project: #08.2_PWM_[Variable_Frequency]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 02.04.2020
 * @Purpose: Основной модуль
 * @Description:
 * В данном проекте реализовано управление скоростью вращения вала двигателя
 * посредством частотно-импульсной модуляции. Модуляция организована
 * на таймере 1 (канал 2). Частота импульсов задаётся положением ручки
 * потенциометра через АЦП.
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

  // Инициализация ШИМ
  PWM_Init();

  // Инициализация двигателя
  HSM_Init();

  // Инициализация переменных
  uint32_t *adc_result = 0;  // Указатель на массив результатов
  uint32_t frequency = 0;    // Расчётная частота

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

        // Вычисление частоты (в герцах)
        frequency = ADC_TO_FRQ(adc_result[4]);

        // Задание частоты
        PWM_SetFrequency(MDR_TIMER1, TIMER_CHANNEL2, frequency);

        // Отображение результатов
        LCD_PrintString("     F = %u Гц", 3, frequency);   // Частота
        LCD_PrintString("     D = %u", 5, adc_result[4]);  // Цифровое значение

        // Запуск цикла преобразований
        MDR_ADC->ADC1_CFG |= ADC1_CFG_REG_ADON;
      }
    }

    // Блок работы с кнопкой
    if ((Wait(&mark[1], 100) == true)) {

      // Обработка кнопки MID
      if (BTN_Handler(BTN_M) == true) {

        // Изменение направления вращения вала
        MDR_PORTF->RXTX ^= PORT_RXTX5_Msk;
      }
    }

  }
}
