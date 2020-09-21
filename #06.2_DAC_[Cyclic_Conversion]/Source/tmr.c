/******************************************************************************
 * @File: tmr.c
 * @Author: Milandr, L.
 * @Project: #06.2_DAC_[Cyclic_Conversion]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 19.05.2020
 * @Purpose: Конфигурация аппаратных таймеров
 ******************************************************************************/

// Подключение заголовка
#include "tmr.h"

// Деинициализация таймера
void TIMER_Reset(MDR_TIMER_TypeDef *MDR_TIMERx)
{
  MDR_TIMERx->CNT          = 0;
  MDR_TIMERx->PSG          = 0;
  MDR_TIMERx->ARR          = 0;
  MDR_TIMERx->CNTRL        = 0;
  MDR_TIMERx->CCR1         = 0;
  MDR_TIMERx->CCR2         = 0;
  MDR_TIMERx->CCR3         = 0;
  MDR_TIMERx->CCR4         = 0;
  MDR_TIMERx->CCR11        = 0;
  MDR_TIMERx->CCR21        = 0;
  MDR_TIMERx->CCR31        = 0;
  MDR_TIMERx->CCR41        = 0;
  MDR_TIMERx->CH1_CNTRL    = 0;
  MDR_TIMERx->CH2_CNTRL    = 0;
  MDR_TIMERx->CH3_CNTRL    = 0;
  MDR_TIMERx->CH4_CNTRL    = 0;
  MDR_TIMERx->CH1_CNTRL1   = 0;
  MDR_TIMERx->CH2_CNTRL1   = 0;
  MDR_TIMERx->CH3_CNTRL1   = 0;
  MDR_TIMERx->CH4_CNTRL1   = 0;
  MDR_TIMERx->CH1_CNTRL2   = 0;
  MDR_TIMERx->CH2_CNTRL2   = 0;
  MDR_TIMERx->CH3_CNTRL2   = 0;
  MDR_TIMERx->CH4_CNTRL2   = 0;
  MDR_TIMERx->CH1_DTG      = 0;
  MDR_TIMERx->CH2_DTG      = 0;
  MDR_TIMERx->CH3_DTG      = 0;
  MDR_TIMERx->CH4_DTG      = 0;
  MDR_TIMERx->BRKETR_CNTRL = 0;
  MDR_TIMERx->STATUS       = 0;
  MDR_TIMERx->IE           = 0;
  MDR_TIMERx->DMA_RE       = 0;
}

// Инициализация таймера
void TIMER_Init(void)
{
  // Включение тактирования
  MDR_RST_CLK->PER_CLOCK |= RST_CLK_PCLK_TIMER1_Msk;
  MDR_RST_CLK->TIM_CLOCK |= RST_CLK_TIM_CLOCK_TIM1_CLK_EN;

  // Деинициализация
  TIMER_Reset(MDR_TIMER1);

  // Делитель тактовой частоты
  MDR_TIMER1->PSG = TIMER_PSG - 1;

  // Общая конфигурация
  MDR_TIMER1->CNTRL = (1 << TIMER_CNTRL_CNT_EN_Pos)      // Работа таймера (включён)
                    | (0 << TIMER_CNTRL_ARRB_EN_Pos)     // Обновление регистра ARR (мгновенное)
                    | (0 << TIMER_CNTRL_DIR_Pos)         // Направление счёта (прямой счёт)
                    | (0 << TIMER_CNTRL_CNT_MODE_Pos)    // Фиксация направления (однонаправленный счёт)
                    | (0 << TIMER_CNTRL_EVENT_SEL_Pos);  // Триггер счёта (тактовые импульсы)

  // Настройка запросов к DMA
  MDR_TIMER1->DMA_RE = TIMER_DMA_RE_CNT_ARR_EVENT_RE;  // Запрос при CNT = ARR
}
