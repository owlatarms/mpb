/******************************************************************************
 * @File: pwm.c
 * @Author: Milandr, L.
 * @Project: #08.2_PWM_[Variable_Frequency]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 02.04.2020
 * @Purpose: Конфигурация аппаратных таймеров в режиме ШИМ
 ******************************************************************************/

// Подключение заголовка
#include "pwm.h"

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

// Инициализация таймера в режиме ШИМ
void PWM_Init(void)
{
  // Включение тактирования порта A и таймера
  MDR_RST_CLK->PER_CLOCK |= RST_CLK_PCLK_PORTA_Msk
                          | RST_CLK_PCLK_TIMER1_Msk;
  MDR_RST_CLK->TIM_CLOCK |= RST_CLK_TIM_CLOCK_TIM1_CLK_EN;

  // Конфигурация линии PA3
  MDR_PORTA->OE     |=  0x00000008;  // Направление (вывод)
  MDR_PORTA->FUNC   &= ~0x000000C0;  // Сброс битов регистра
  MDR_PORTA->FUNC   |=  0x00000080;  // Функция (альтернативная)
  MDR_PORTA->ANALOG |=  0x00000008;  // Режим (цифровой)
  MDR_PORTA->PULL   &= ~0x00080008;  // Подтяжка (отключена)
  MDR_PORTA->PD     &= ~0x00080008;  // Управление (не используется)
  MDR_PORTA->PWR    |=  0x000000C0;  // Крутизна (высокая)
  MDR_PORTA->GFEN   &= ~0x00000008;  // Фильтр (не используется)

  // Деинициализация таймера
  TIMER_Reset(MDR_TIMER1);

  // Конфигурация таймера
  MDR_TIMER1->CNTRL = (0 << TIMER_CNTRL_CNT_EN_Pos)      // Работа таймера (пока отключён)
                    | (1 << TIMER_CNTRL_ARRB_EN_Pos)     // Обновление регистра ARR (при перезагрузке)
                    | (0 << TIMER_CNTRL_DIR_Pos)         // Направление счёта (прямой счёт)
                    | (0 << TIMER_CNTRL_CNT_MODE_Pos)    // Фиксация направления (однонаправленный счёт)
                    | (0 << TIMER_CNTRL_EVENT_SEL_Pos);  // Триггер счёта (тактовые импульсы)

  // Конфигурация канала (CH2)
  MDR_TIMER1->CH2_CNTRL  = (6 << TIMER_CH_CNTRL_OCCM_Pos)       // Формат выходного сигнала (REF6)
                         | (0 << TIMER_CH_CNTRL_CAP_NPWM_Pos);  // Режим работы канала (ШИМ)

  // Конфигурация линий канала
  // Основная линия
  MDR_TIMER1->CH2_CNTRL1 = (1 << TIMER_CH_CNTRL1_SELOE_Pos)   // Разрешение генерации выходного сигнала (разрешена)
                         | (2 << TIMER_CH_CNTRL1_SELO_Pos)    // Управление выходом (REF)
                         | (0 << TIMER_CH_CNTRL1_INV_Pos)     // Инверсия линии (отключена)

  // ...Комплементарная линия
                         | (0 << TIMER_CH_CNTRL1_NSELOE_Pos)  // Разрешение генерации выходного сигнала (запрещена)
                         | (0 << TIMER_CH_CNTRL1_NSELO_Pos)   // Управление выходом (отключено)
                         | (0 << TIMER_CH_CNTRL1_NINV_Pos);   // Инверсия линии (отключена)

  // Делитель тактовой частоты таймера
  MDR_TIMER1->PSG = 0;

  // Период перезагрузки таймера
  MDR_TIMER1->ARR = 0;

  // Запуск таймера
  MDR_TIMER1->CNTRL |= TIMER_CNTRL_CNT_EN;
}

// Установка частоты импульсов
void PWM_SetFrequency(MDR_TIMER_TypeDef *MDR_TIMERx, timer_channel_t channel, uint32_t frequency)
{
  // Инициализация переменных
  uint32_t psg = 1;  // Делитель
  uint32_t arr = 0;  // Период

  // Верификация аргумента
  if (frequency < 1) {
    MDR_TIMERx->CNTRL &= ~TIMER_CNTRL_CNT_EN;
    return;
  } else if (frequency > SystemCoreClock / (TIMER_ARR_MIN + 1)) {
    frequency = SystemCoreClock / (TIMER_ARR_MIN + 1);
  }

  // Исходное вычисление периода
  arr = SystemCoreClock / (frequency * psg) - 1;

  // Цикл вычисления допустимых значений периода и делителя;
  // выполняется, пока: период вне допуска & делитель в допуске
  while (arr >= TIMER_ARR_MAX
     && (psg <= TIMER_PSG_MAX)) {

    // Увеличение делителя в 2 раза
    psg *= 2;

    // Повторное вычисление периода
    arr = SystemCoreClock / (frequency * psg) - 1;
  }

  // Установка делителя
  MDR_TIMERx->PSG = psg - 1;

  // Установка периода
  MDR_TIMERx->ARR = arr;

  // Установка скважности
  PWM_SetPowerCycle(MDR_TIMERx, channel, PWM_S);

  // Запуск таймера
  MDR_TIMERx->CNTRL |= TIMER_CNTRL_CNT_EN;
}

// Установка скважности импульсов
void PWM_SetPowerCycle(MDR_TIMER_TypeDef *MDR_TIMERx, timer_channel_t channel, uint32_t power_cycle)
{
  // Флаг работы в режиме максимальной мощности
  static bool flg_max_pwr = false;

  // Определение регистров для настройки
  uint32_t *timer_ccr   = (uint32_t*)&MDR_TIMERx->CCR1 + channel;
  uint32_t *timer_cntrl = (uint32_t*)&MDR_TIMERx->CH1_CNTRL + channel;

  // Вычисление ширины импульсов (в квантах таймера),
  // и запись вычисленного значения в регистр сравнения (CCR)
  *timer_ccr = MDR_TIMERx->ARR * power_cycle / 100;

  // Если требуется получить максимульную мощность (S = 100%) -
  // канал таймера переводится в режим REF5
  if (power_cycle >= 100 && flg_max_pwr == false) {
    *timer_cntrl &= ~TIMER_CH_CNTRL_OCCM_Msk;
    *timer_cntrl |= (5 << TIMER_CH_CNTRL_OCCM_Pos);
    flg_max_pwr = true;
  }

  // Если требуется вернуться в штатный режим регулирования -
  // канал таймера переводится обратно в режим REF6
  else if (power_cycle < 100 && flg_max_pwr == true) {
    *timer_cntrl &= ~TIMER_CH_CNTRL_OCCM_Msk;
    *timer_cntrl |= (6 << TIMER_CH_CNTRL_OCCM_Pos);
    flg_max_pwr = false;
  }
}

// Инициализация двигателя
// (Hybrid Stepper Motor)
void HSM_Init(void)
{
  // Включение тактирования порта F
  MDR_RST_CLK->PER_CLOCK |= RST_CLK_PCLK_PORTF_Msk;

  // Конфигурация линий PF4 (EN) и PF5 (DIR)
  MDR_PORTF->OE     |=  0x00000030;  // Направление (вывод)
  MDR_PORTF->FUNC   &= ~0x00000F00;  // Функция (ввод-вывод)
  MDR_PORTF->ANALOG |=  0x00000030;  // Режим (цифровой)
  MDR_PORTF->PULL   &= ~0x00300030;  // Подтяжка (отключена)
  MDR_PORTF->PD     &= ~0x00300030;  // Управление (драйвер)
  MDR_PORTF->PWR    |=  0x00000F00;  // Крутизна (высокая)
  MDR_PORTF->GFEN   &= ~0x00000030;  // Фильтр (не используется)

  // Включение двигателя
  // (активный уровень - низкий)
  MDR_PORTF->RXTX &= ~PORT_RXTX4_Msk;

  // Установка направления вращения вала
  // (по часовой стрелке)
  MDR_PORTF->RXTX &= ~PORT_RXTX5_Msk;
}
