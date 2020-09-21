/******************************************************************************
 * @File: tmr.c
 * @Author: Milandr, L.
 * @Project: #03.1_Timer_[General_Purpose]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 06.04.2020
 * @Purpose: Конфигурация аппаратных таймеров
 ******************************************************************************/

// Подключение заголовка
#include "tmr.h"

// Инициализация глобальных переменных
volatile bool     flg_tmr_eoc = false;  // Флаг завершения счёта
volatile uint32_t tmr_rld_cnt = 0;      // Счётчик перезагрузок таймера

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

// Инициализация циклического таймера (TIMER1)
// для установки периодов исполнения операций
void TIMER1_Init(void)
{
  // Включение тактирования
  MDR_RST_CLK->PER_CLOCK |= RST_CLK_PCLK_TIMER1_Msk;
  MDR_RST_CLK->TIM_CLOCK |= RST_CLK_TIM_CLOCK_TIM1_CLK_EN;

  // Деинициализация
  TIMER_Reset(MDR_TIMER1);

  // Делитель тактовой частоты
  MDR_TIMER1->PSG = 2000 - 1;

  // Период перезагрузки;
  // значение выбрано для периода в 1 секунду по формуле:
  // Tr = (PSG / CLK) * (ARR + 1)
  MDR_TIMER1->ARR = 39999;

  // Общая конфигурация
  MDR_TIMER1->CNTRL = (0 << TIMER_CNTRL_CNT_EN_Pos)      // Работа таймера (пока отключён)
                    | (0 << TIMER_CNTRL_ARRB_EN_Pos)     // Обновление регистра ARR (мгновенное)
                    | (0 << TIMER_CNTRL_DIR_Pos)         // Направление счёта (прямой счёт)
                    | (0 << TIMER_CNTRL_CNT_MODE_Pos)    // Фиксация направления (однонаправленный счёт)
                    | (0 << TIMER_CNTRL_EVENT_SEL_Pos);  // Триггер счёта (тактовые импульсы)

  // Настройка запросов на обработку прерываний
  MDR_TIMER1->IE = TIMER_IE_CNT_ARR_EVENT_IE;  // Прерывание при CNT = ARR

  // Конфигурация контроллера NVIC
  NVIC_SetPriority(Timer1_IRQn, 5);  // Приоритет прерываний
  NVIC_EnableIRQ(Timer1_IRQn);       // Разрешение обработки прерываний
}

// Инициализация метрического таймера (TIMER2)
// для измерения времени исполнения операций
void TIMER2_Init(void)
{
  // Включение тактирования
  MDR_RST_CLK->PER_CLOCK |= RST_CLK_PCLK_TIMER2_Msk;
  MDR_RST_CLK->TIM_CLOCK |= RST_CLK_TIM_CLOCK_TIM2_CLK_EN;

  // Деинициализация
  TIMER_Reset(MDR_TIMER2);

  // Делитель тактовой частоты;
  // единичный делитель позволяет считать время в тактах ядра
  MDR_TIMER2->PSG = 1 - 1;

  // Период перезагрузки (максимальный)
  MDR_TIMER2->ARR = 65535;

  // Общая конфигурация
  MDR_TIMER2->CNTRL = (0 << TIMER_CNTRL_CNT_EN_Pos)      // Работа таймера (пока отключён)
                    | (0 << TIMER_CNTRL_ARRB_EN_Pos)     // Обновление регистра ARR (мгновенное)
                    | (0 << TIMER_CNTRL_DIR_Pos)         // Направление счёта (прямой счёт)
                    | (0 << TIMER_CNTRL_CNT_MODE_Pos)    // Фиксация направления счёта (однонаправленный счёт)
                    | (0 << TIMER_CNTRL_EVENT_SEL_Pos);  // Источник событий для счёта (тактовые импульсы)

  // Настройка запросов на обработку прерываний
  MDR_TIMER2->IE = TIMER_IE_CNT_ARR_EVENT_IE;  // Прерывание при CNT = ARR

  // Конфигурация контроллера NVIC
  NVIC_SetPriority(Timer2_IRQn, 6);  // Приоритет прерываний
  NVIC_EnableIRQ(Timer2_IRQn);       // Разрешение обработки прерываний
}

// Обработчик прерывания от TIMER1
void Timer1_IRQHandler(void)
{
  // Сброс запроса на обработку прерывания
  MDR_TIMER1->STATUS &= ~TIMER_IE_CNT_ARR_EVENT_IE;

  // Установка флага завершения счёта
  flg_tmr_eoc = true;
}

// Обработчик прерывания от TIMER2
void Timer2_IRQHandler(void)
{
  // Сброс запроса на обработку прерывания
  MDR_TIMER2->STATUS &= ~TIMER_IE_CNT_ARR_EVENT_IE;

  // Регистрация перезагрузки таймера
  tmr_rld_cnt++;
}
