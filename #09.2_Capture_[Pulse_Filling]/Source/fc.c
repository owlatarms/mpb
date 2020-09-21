/******************************************************************************
 * @File: fc.c
 * @Author: Milandr, L.
 * @Project: #09.2_Capture_[Pulse_Filling]
 * @Version: 1.0.0
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 24.07.2020
 * @Purpose: Реализация частотомера с алгоритмом импульсного заполнения
 ******************************************************************************/

// Подключение заголовка
#include "fc.h"

// Инициализация глобальных переменных
volatile uint32_t fc_sample[FC_SRM] = {0};  // Буфер выборки отсчётов
volatile bool flg_fc_eoc = false;           // Флаг завершения измерения

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
// (таймер в режиме захвата)
void FC_Init(void)
{
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
  MDR_TIMER2->PSG = TIMER_PSG - 1;

  // Период перезагрузки таймера (максимальный)
  MDR_TIMER2->ARR = 65535;

  // Конфигурация таймера
  MDR_TIMER2->CNTRL = (1 << TIMER_CNTRL_CNT_EN_Pos)      // Работа таймера (включён)
                    | (1 << TIMER_CNTRL_ARRB_EN_Pos)     // Обновление регистра ARR (при перезагрузке)
                    | (0 << TIMER_CNTRL_DIR_Pos)         // Направление счёта (прямой счёт)
                    | (0 << TIMER_CNTRL_FDTS_Pos)        // Частота фильтрации (не используется)
                    | (0 << TIMER_CNTRL_CNT_MODE_Pos)    // Фиксация направления (однонаправленный счёт)
                    | (0 << TIMER_CNTRL_EVENT_SEL_Pos);  // Триггер счёта (тактовые импульсы)

  // Конфигурация канала (CH3)
  MDR_TIMER2->CH3_CNTRL = (3 << TIMER_CH_CNTRL_CHFLTR_Pos)     // Фильтрация сигнала (TIM_CLK / 8)
                        | (0 << TIMER_CH_CNTRL_CHSEL_Pos)      // Триггер захвата (фронт сигнала)
                        | (1 << TIMER_CH_CNTRL_CAP_NPWM_Pos);  // Режим работы канала (захват)

  // Конфигурация линий канала
  // Прямая линия
  MDR_TIMER2->CH3_CNTRL1 = (0 << TIMER_CH_CNTRL1_SELOE_Pos)  // Разрешение генерации выходного сигнала (запрещена)
                         | (0 << TIMER_CH_CNTRL1_SELO_Pos)   // Управление выходом (не используется)
                         | (0 << TIMER_CH_CNTRL1_INV_Pos);   // Инверсия линии (отключена)

  // Конфигурация сравнения
  MDR_TIMER2->CH3_CNTRL2 = (1 << TIMER_CH_CNTRL2_CCRRLD_Pos);  // Обновление регистра CCR (мгновенное)

  // Настройка запросов на обработку прерываний
  MDR_TIMER2->DMA_RE = (4 << TIMER_DMA_RE_CCR_CAP_EVENT_RE_Pos);  // Захват на канале 3

  // Инициализация DMA
  DMA_Init();

  // Вычисление доверительного интервала измерений
  // Нижняя граница: частота, при которой возникает переполнение таймера
  uint32_t lower_limit = SystemCoreClock / (TIMER_PSG * (MDR_TIMER2->ARR + 1));

  // Верхняя граница: частота, при которой относительная погрешность достигает значения 1%
  uint32_t upper_limit = SystemCoreClock / (TIMER_PSG * 100);

  // Отображение интервала
  LCD_PrintString("   %u - %u Hz", 7, lower_limit, upper_limit);
}

// Измерение частоты
void FC_Count(uint32_t *frequency, double *epsilon)
{
  // Инициализация переменных
  static fc_fsm_t fsm_state = FC_FSM_RDY;  // Состояние процесса измерения
  static uint32_t timeout = 0;               // Квантовая метка тайм-аута
  
  // Состояние «Измеритель готов»
  if (fsm_state == FC_FSM_RDY) {

    // Снятие маски с канала: начало цикла измерений
    MDR_DMA->CHNL_REQ_MASK_CLR = (1 << 11);

    // Переход к состоянию «Измеритель занят»
    fsm_state = FC_FSM_BSY;
  }

  // Состояние «Измеритель занят»
  if (fsm_state == FC_FSM_BSY) {

    // Запуск тайм-аута измерений
    // (тайм-аут используется в случае если на канале отсутствуют
    // входные импульсы, т.е. частота импульсов равна нулю)
    Wait(&timeout, FC_TIMEOUT);

    // Если процесс измерения завершён...
    if (flg_fc_eoc == true) {

      // Сброс флага
      flg_fc_eoc = false;

      // Вычисление разности отсчётов между фронтами сигнала
      uint32_t fc_diff[FC_SRM] = {0};  // Инициализация массива
      fc_diff[0] = fc_sample[0];       // Опорный элемент - нулевой

      // Цикл вычислений: если i-ый элемент больше предыдущего - вычисляется разность;
      // иначе делается поправка на новый цикл отсчёта ввиду перезагрузки таймера
      for (int i = 1; i < FC_SRM; i++) {
        if (fc_sample[i] > fc_sample[i - 1]) {
          fc_diff[i] = fc_sample[i] - fc_sample[i - 1];
        } else {
          fc_diff[i] = fc_sample[i] + MDR_TIMER2->ARR - fc_sample[i - 1];
        }
      }

      // Сортировка массива
      QuickSort((int*)fc_diff, FC_SRM);

      // Медианная фильтрация
      *frequency = FC_FRQ / fc_diff[FC_SRM / 2];

      // Вычисление относительной погрешности
      *epsilon = 100.0F * *frequency / FC_FRQ;

      // Возврат к состоянию «Измеритель готов»
      fsm_state = FC_FSM_RDY;

      // Сброс тайм-аута
      timeout = 0;
    }

    // Если процесс измерения не завершился до тайм-аута...
    else if (Wait(&timeout, 1000) == true) {

      // Установка нулевых значений
      *frequency = 0;
      *epsilon   = 0;

      // Возврат к состоянию «Измеритель готов»
      fsm_state = FC_FSM_RDY;
    }
  }
}
