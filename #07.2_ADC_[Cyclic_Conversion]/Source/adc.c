/******************************************************************************
 * @File: adc.c
 * @Author: Milandr, L.
 * @Project: #07.2_ADC_[Cyclic_Conversion]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 18.03.2020
 * @Purpose: Управление аналого-цифровым преобразователем
 ******************************************************************************/

// Подключение заголовка
#include "adc.h"

// Инициализация глобальных переменных
volatile uint32_t adc_sample[ADC_SRM] = {0};  // Буфер хранения выборки преобразований
volatile bool flg_adc_eoc = false;            // Флаг завершения цикла преобразований

// Деинициализация АЦП
void ADC_Reset(void)
{
  // Деинициализация первого АЦП
  MDR_ADC->ADC1_CFG     = 0;
  MDR_ADC->ADC1_H_LEVEL = 0;
  MDR_ADC->ADC1_L_LEVEL = 0;
  MDR_ADC->ADC1_RESULT;
  MDR_ADC->ADC1_STATUS  = 0;
  MDR_ADC->ADC1_CHSEL   = 0;

  // Деинициализация второго АЦП
  MDR_ADC->ADC2_CFG     = 0;
  MDR_ADC->ADC2_H_LEVEL = 0;
  MDR_ADC->ADC2_L_LEVEL = 0;
  MDR_ADC->ADC2_RESULT;
  MDR_ADC->ADC2_STATUS  = 0;
  MDR_ADC->ADC2_CHSEL   = 0;
}

// Инициализация АЦП
void ADC_Init(void)
{
  // Включение тактирования порта D и АЦП
  MDR_RST_CLK->PER_CLOCK |= RST_CLK_PCLK_PORTD_Msk
                          | RST_CLK_PCLK_ADC_Msk;

  // Конфигурация линии PD2
  MDR_PORTD->OE     &= ~0x00000002;
  MDR_PORTD->FUNC   &= ~0x00000030;
  MDR_PORTD->ANALOG &= ~0x00000002;  // Режим (аналоговый)
  MDR_PORTD->PULL   &= ~0x00020002;  // Подтяжка (отключена)
  MDR_PORTD->PD     &= ~0x00020002;
  MDR_PORTD->PWR    &= ~0x00000030;
  MDR_PORTD->GFEN   &= ~0x00000002;

  // Деинициализация АЦП
  ADC_Reset();

  // Конфигурация АЦП
  MDR_ADC->ADC1_CFG = (0 << ADC1_CFG_REG_ADON_Pos)     // Работа АЦП (пока отключён)
                    | (0 << ADC1_CFG_REG_CLKS_Pos)     // Источник тактирования АЦП (CPU)
                    | (1 << ADC1_CFG_REG_SAMPLE_Pos)   // Способ запуска АЦП (циклический)
                    | (2 << ADC1_CFG_REG_CHS_Pos)      // Целевой канал преобразователя (ADC2)
                    | (0 << ADC1_CFG_REG_CHCH_Pos)     // Режим последовательного переключения каналов (отключён)
                    | (0 << ADC1_CFG_REG_RNGC_Pos)     // Контроль границ преобразования (отключён)
                    | (0 << ADC1_CFG_M_REF_Pos)        // Источник опорного напряжения (внутренний)
                    | (3 << ADC1_CFG_REG_DIVCLK_Pos)   // Делитель тактовой частоты АЦП (2^3 = 8)
                    | (0 << ADC1_CFG_SYNC_CONVER_Pos)  // Режим запуска двух АЦП (независимый)

  // ...Конфигурация датчика температуры и внутреннего источника напряжения 1.23 В
                    | (0 << ADC1_CFG_TS_EN_Pos)        // Работа блока датчика температуры и внутреннего источника напряжения 1.23 В (отключён)
                    | (0 << ADC1_CFG_TS_BUF_EN_Pos)    // Работа усилителя для датчика температуры и внутреннего источника напряжения 1.23 В (отключён)
                    | (0 << ADC1_CFG_SEL_TS_Pos)       // Преобразование сигнала с датчика температуры (отключено)
                    | (0 << ADC1_CFG_SEL_VREF_Pos)     // Преобразование сигнала с внутреннего источника напряжения 1.23 В (отключено)
                    | (0 << ADC1_CFG_TR_Pos)           // Подстройка напряжения внутреннего источника 1.23 В

  // ...Настройка задержек при преобразовании
                    | (7 << ADC1_CFG_DELAY_GO_Pos)     // Дополнительная задержка при выборе канала (8 тактов ядра)
                    | (0 << ADC1_CFG_DELAY_ADC_Pos);   // Разность фаз между циклами преобразователей (не используется)

  // Инициализация DMA
  DMA_Init();
}

// Вычисление среднего арифметического выборки
uint32_t ADC_GetAverage(volatile uint32_t *buffer)
{
  // Результат преобразования
  uint32_t adc_result = 0;

  // Суммирование элементов выборки
  for (int i = 0; i < ADC_SRM; i++) {
    adc_result += buffer[i] & ADC_RESULT_Msk;
  }

  // Усредненение выборки
  adc_result /= ADC_SRM;

  // Возврат результата преобразования
  return adc_result;
}
