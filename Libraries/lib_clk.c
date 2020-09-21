/******************************************************************************
 * @File: lib_clk.c
 * @Author: Milandr, L.
 * @Project: Library
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 12.03.2020
 * @Purpose: Конфигурация системы тактирования микроконтроллера
 * @Description:
 * На отладочной плате «МилКиТЭС» установлен кварцевый резонатор 10 МГц;
 * для получения тактовой частоты 80 МГц сигнал с генератора HSE следует
 * умножить на 8, задействовав блок фазовой автоподстройки частоты (PLL)
 ******************************************************************************/

// Подключение заголовка
#include "lib_clk.h"

// Исходная тактовая частота системы (8 МГц)
uint32_t SystemCoreClock = 8000000;

// Деинициализация системы тактирования
void CPU_Reset(void)
{
  // Сброс настроек системы тактирования
  MDR_RST_CLK->PLL_CONTROL   = 0;
  MDR_RST_CLK->HS_CONTROL    = 0;
  MDR_RST_CLK->CPU_CLOCK     = 0;
  MDR_RST_CLK->USB_CLOCK     = 0;
  MDR_RST_CLK->ADC_MCO_CLOCK = 0;
  MDR_RST_CLK->RTC_CLOCK     = 0;
  MDR_RST_CLK->CAN_CLOCK     = 0;
  MDR_RST_CLK->TIM_CLOCK     = 0;
  MDR_RST_CLK->UART_CLOCK    = 0;
  MDR_RST_CLK->SSP_CLOCK     = 0;

  // Отключение тактирования всех блоков,
  // кроме системы тактирования и батарейного домена
  MDR_RST_CLK->PER_CLOCK = RST_CLK_PCLK_RST_CLK_Msk
                         | RST_CLK_PCLK_BKP_Msk;
}

// Инициализация системы тактирования
void CPU_Init(void)
{
  // Деинициализация системы тактирования
  CPU_Reset();

  // Включение генератора HSE
  MDR_RST_CLK->HS_CONTROL = (1 << RST_CLK_HS_CONTROL_HSE_ON_Pos);

  // Ожидание входа генератора HSE в рабочий режим
  while ((MDR_RST_CLK->CLOCK_STATUS & RST_CLK_CLOCK_STATUS_HSE_RDY) == false);

  // Инициализация блока PLL
  MDR_RST_CLK->PLL_CONTROL = (1 << RST_CLK_PLL_CONTROL_PLL_CPU_ON_Pos)    // Включение блока PLL
                           | (7 << RST_CLK_PLL_CONTROL_PLL_CPU_MUL_Pos);  // Задание коэффициента умножения (7 + 1 = 8)

  // Ожидание входа блока PLL в рабочий режим
  while ((MDR_RST_CLK->CLOCK_STATUS & RST_CLK_CLOCK_STATUS_PLL_CPU_RDY) == false);

  // Настройка тракта формирования тактовой частоты
  MDR_RST_CLK->CPU_CLOCK = (2 << RST_CLK_CPU_CLOCK_CPU_C1_SEL_Pos)  // CPU_C1 = HSE
                         | (1 << RST_CLK_CPU_CLOCK_CPU_C2_SEL_Pos)  // CPU_C2 = PLLCPUo
                         | (0 << RST_CLK_CPU_CLOCK_CPU_C3_SEL_Pos)  // CPU_C3 = CPU_C2
                         | (1 << RST_CLK_CPU_CLOCK_HCLK_SEL_Pos);   // HCLK   = CPU_C3

  // Обновление значения тактовой частоты
  SystemCoreClock = 80000000;

  // Отключение генераторов HSI и LSI
  MDR_BKP->REG_0F &= ~(BKP_REG_0F_LSI_ON | BKP_REG_0F_HSI_ON);
}
