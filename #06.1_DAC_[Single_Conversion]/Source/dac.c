/******************************************************************************
 * @File: dac.c
 * @Author: Milandr, L.
 * @Project: #06.1_DAC_[Single_Conversion]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 15.05.2020
 * @Purpose: Конфигурация цифро-аналогового преобразователя
 ******************************************************************************/

// Подключение заголовка
#include "dac.h"

// Деинициализация ЦАП
void DAC_Reset(void)
{
  MDR_DAC->DAC2_DATA = 0;
  MDR_DAC->CFG       = 0;
}

// Инициализация ЦАП
void DAC_Init(void)
{
  // Включение тактирования порта E и ЦАП
  MDR_RST_CLK->PER_CLOCK |= RST_CLK_PCLK_PORTE_Msk
                          | RST_CLK_PCLK_DAC_Msk;

  // Конфигурация линии PE0
  MDR_PORTE->OE     &= ~0x00000001;
  MDR_PORTE->FUNC   &= ~0x00000003;
  MDR_PORTE->ANALOG &= ~0x00000001;  // Режим (аналоговый)
  MDR_PORTE->PULL   &= ~0x00000001;  // Подтяжка (отключена)
  MDR_PORTE->PD     &= ~0x00010001;
  MDR_PORTE->PWR    &= ~0x00000003;
  MDR_PORTE->GFEN   &= ~0x00000001;

  // Деинициализация ЦАП
  DAC_Reset();

  // Конфигурация ЦАП
  MDR_DAC->CFG = (0 << DAC_CFG_M_REF1_Pos)    // Источник опорного напряжения (внутренний)
               | (1 << DAC_CFG_ON_DAC1_Pos);  // Работа ЦАП (включён)
}

// Установка напряжения ЦАП
void DAC_SetVoltage(float *voltage)
{
  // Верификация аргумента
  if (*voltage < 0.00) {
    *voltage = 0.00;
  } else if (*voltage > DAC_REF) {
    *voltage = DAC_REF;
  }

  // Вычисление кода и его запись в ЦАП
  MDR_DAC->DAC2_DATA = VLT_TO_DAC(*voltage);

  // Отображение напряжения и кода
  LCD_PrintString("    V = %.2f V", 3, *voltage);
  LCD_PrintString("    D = %u", 5, MDR_DAC->DAC2_DATA);
}
