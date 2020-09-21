/******************************************************************************
 * @File: fc.c
 * @Author: Milandr, L.
 * @Project: #09.1_Capture_[Discrete_Count]
 * @Version: 1.0.0
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 20.07.2020
 * @Purpose: Реализация частотомера с алгоритмом дискретного счёта
 ******************************************************************************/

// Подключение заголовка
#include "fc.h"

// Инициализация глобальных переменных
volatile bool     flg_tmr_eoc = false;  // Флаг завершения периода измерения
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

// Инициализация частотомера
// (таймеры в режимах внутреннего и внешнего тактирования)
void FC_Init(void)
{
  /* Блок инициализации таймера (TIMER1)
     для установки периода измерения */
  // Включение тактирования таймера
  MDR_RST_CLK->PER_CLOCK |= RST_CLK_PCLK_TIMER1_Msk;
  MDR_RST_CLK->TIM_CLOCK |= RST_CLK_TIM_CLOCK_TIM1_CLK_EN;

  // Деинициализация таймера
  TIMER_Reset(MDR_TIMER1);

  // Делитель тактовой частоты таймера
  MDR_TIMER1->PSG = TIMER_PSG - 1;

  // Период перезагрузки таймера
  MDR_TIMER1->ARR = TIME_TO_CNT(FC_GATE_TIME);

  // Конфигурация таймера
  MDR_TIMER1->CNTRL = (0 << TIMER_CNTRL_CNT_EN_Pos)      // Работа таймера (пока отключён)
                    | (1 << TIMER_CNTRL_ARRB_EN_Pos)     // Обновление регистра ARR (при перезагрузке)
                    | (0 << TIMER_CNTRL_DIR_Pos)         // Направление счёта (прямой счёт)
                    | (0 << TIMER_CNTRL_CNT_MODE_Pos)    // Фиксация направления (однонаправленный счёт)
                    | (0 << TIMER_CNTRL_EVENT_SEL_Pos);  // Триггер счёта (тактовые импульсы)

  // Настройка запросов на обработку прерываний
  MDR_TIMER1->IE = TIMER_IE_CNT_ARR_EVENT_IE;  // Прерывание при CNT = ARR

  // Конфигурация контроллера NVIC
  NVIC_SetPriority(Timer1_IRQn, 5);  // Приоритет прерываний
  NVIC_EnableIRQ(Timer1_IRQn);       // Разрешение обработки прерываний

  // Отключение тактирования
  // для дальнейшего синхронного запуска
  MDR_RST_CLK->TIM_CLOCK &= ~RST_CLK_TIM_CLOCK_TIM1_CLK_EN;

  // Включение таймера
  MDR_TIMER1->CNTRL |= TIMER_CNTRL_CNT_EN;

  /* Блок инициализации таймера (TIMER2)
     для подсчёта внешних импульсов */
  // Включение тактирования порта А и таймера
  MDR_RST_CLK->PER_CLOCK |= RST_CLK_PCLK_PORTA_Msk
                          | RST_CLK_PCLK_TIMER2_Msk;
  MDR_RST_CLK->TIM_CLOCK |= RST_CLK_TIM_CLOCK_TIM2_CLK_EN;

  // Конфигурация линии PA5 (TMR2_CH3)
  MDR_PORTA->OE     &=  0x00000020;  // Направление (ввод)
  MDR_PORTA->FUNC   |=  0x00000C00;  // Функция (переопределённая)
  MDR_PORTA->ANALOG |=  0x00000020;  // Режим (цифровой)
  MDR_PORTA->PULL   &= ~0x00200020;  // Подтяжка (отключена)
  MDR_PORTA->PD     &= ~0x00200020;  // Управление (не используется)
  MDR_PORTA->PWR    &= ~0x00000C00;  // Крутизна (не используется)
  MDR_PORTA->GFEN   &= ~0x00000020;  // Фильтр (не используется)

  // Деинициализация таймера
  TIMER_Reset(MDR_TIMER2);

  // Делитель тактовой частоты таймера
  MDR_TIMER2->PSG = 1 - 1;

  // Период перезагрузки таймера (максимальный)
  MDR_TIMER2->ARR = 65535;

  // Конфигурация таймера
  MDR_TIMER2->CNTRL = (0 << TIMER_CNTRL_CNT_EN_Pos)      // Работа таймера (пока отключён)
                    | (1 << TIMER_CNTRL_ARRB_EN_Pos)     // Обновление регистра ARR (при перезагрузке)
                    | (0 << TIMER_CNTRL_DIR_Pos)         // Направление счёта (прямой счёт)
                    | (0 << TIMER_CNTRL_FDTS_Pos)        // Частота фильтрации (не используется)
                    | (2 << TIMER_CNTRL_CNT_MODE_Pos)    // Фиксация направления (однонаправленный счёт)
                    | (6 << TIMER_CNTRL_EVENT_SEL_Pos);  // Триггер счёта (импульсы канале 3)

  // Конфигурация канала (CH3)
  MDR_TIMER2->CH3_CNTRL = (3 << TIMER_CH_CNTRL_CHFLTR_Pos);  // Фильтрация сигнала (TIM_CLK / 8)

  // Конфигурация линий канала
  // Прямая линия
  MDR_TIMER2->CH3_CNTRL1 = (0 << TIMER_CH_CNTRL1_SELOE_Pos)  // Генерация выходного сигнала (отключена)
                         | (0 << TIMER_CH_CNTRL1_SELO_Pos)   // Управление выходом (не используется)
                         | (0 << TIMER_CH_CNTRL1_INV_Pos);   // Инверсия линии (отключена)

  // Настройка запросов на обработку прерываний
  MDR_TIMER2->IE = TIMER_IE_CNT_ARR_EVENT_IE;  // Прерывание при CNT = ARR

  // Конфигурация контроллера NVIC
  NVIC_SetPriority(Timer2_IRQn, 6);  // Приоритет прерываний
  NVIC_EnableIRQ(Timer2_IRQn);       // Разрешение обработки прерываний

  // Отключение тактирования счётчика
  // для дальнейшего синхронного запуска
  MDR_RST_CLK->TIM_CLOCK &= ~RST_CLK_TIM_CLOCK_TIM2_CLK_EN;

  // Включение таймера
  MDR_TIMER2->CNTRL |= TIMER_CNTRL_CNT_EN;
}

// Измерение частоты
void FC_Count(uint32_t *frequency, double *epsilon)
{
  // Состояние процесса измерения
  static fc_fsm_t fsm_state = FC_FSM_RDY;
  
  // Состояние «Измеритель готов»
  if (fsm_state == FC_FSM_RDY) {

    // Сброс счётчиков
    MDR_TIMER1->CNT = 0;
    MDR_TIMER2->CNT = 0;
    tmr_rld_cnt = 0;

    // Синхронный запуск таймеров
    MDR_RST_CLK->TIM_CLOCK |= RST_CLK_TIM_CLOCK_TIM1_CLK_EN
                            | RST_CLK_TIM_CLOCK_TIM2_CLK_EN;

    // Переход к состоянию «Измеритель занят»
    fsm_state = FC_FSM_BSY;
  }

  // Состояние «Измеритель занят»
  if (fsm_state == FC_FSM_BSY) {

    // Если период измерения завершён...
    if (flg_tmr_eoc == true) {

      // Сброс флага
      flg_tmr_eoc = false;

      // Вычисление частоты импульсов
      *frequency = (uint32_t)((MDR_TIMER2->CNT + tmr_rld_cnt * (MDR_TIMER2->ARR + 1)) / FC_GATE_TIME);

      // Вычисление относительной погрешности
      if (*frequency != 0) {
        *epsilon = 100 / (*frequency * FC_GATE_TIME);
      } else {
        *epsilon = 0;
      }

      // Возврат к состоянию «Измеритель готов»
      fsm_state = FC_FSM_RDY;
    }
  }
}

// Обработчик прерывания от TIMER1
void Timer1_IRQHandler(void)
{
  // Остановка таймеров
  MDR_RST_CLK->TIM_CLOCK &= ~RST_CLK_TIM_CLOCK_TIM1_CLK_EN
                          & ~RST_CLK_TIM_CLOCK_TIM2_CLK_EN;

  // Сброс запроса на обработку прерывания
  MDR_TIMER1->STATUS &= ~TIMER_IE_CNT_ARR_EVENT_IE;

  // Установка флага завершения периода измерения
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
