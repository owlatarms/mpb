/******************************************************************************
 * @File: dac.c
 * @Author: Milandr, L.
 * @Project: #06.2_DAC_[Cyclic_Conversion]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 19.05.2020
 * @Purpose: Конфигурация цифро-аналогового преобразователя
 ******************************************************************************/

// Подключение заголовка
#include "dac.h"

// Буфер дискретных значений сигнала
volatile uint16_t dac_sample[DAC_SRM] = {0};

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

  // Инициализация DMA
  DMA_Init();

  // Инициализация таймера
  TIMER_Init();
}

// Генерация дискретного сигнала
void DAC_GenerateWaveform(signal_t *signal)
{
  // Верификация параметров сигнала
  DAC_AssertWaveform(signal);

  // Остановка таймера
  MDR_TIMER1->CNTRL &= ~TIMER_CNTRL_CNT_EN;

  // Генерация дискретного сигнала
  switch (signal->waveform) {

    // Синусоидальная форма
    case SINE:
      for (int i = 0; i < DAC_SRM; i++) {
        signal->buffer[i] = VLT_TO_DAC(signal->constant + signal->amplitude * sin(2 * PI / DAC_SRM * i));
      }
      break;

    // Прямоугольная форма
    case SQUARE:
      for (int i = 0; i < DAC_SRM; i++) {
        signal->buffer[i] = 0;
      }
      break;

    // Пилообразная форма
    case SAWTOOTH:
      for (int i = 0; i < DAC_SRM; i++) {
        signal->buffer[i] = VLT_TO_DAC(signal->constant - signal->amplitude + 2 * signal->amplitude / DAC_SRM * i);
      }
      break;

    // Треугольная форма
    case TRIANGLE:
      for (int i = 0; i < DAC_SRM; i++) {
        signal->buffer[i] = 0;
      }
      break;

    // Шумоподобная форма
    case NOISE:
      for (int i = 0; i < DAC_SRM; i++) {
        signal->buffer[i] = 0;
      }
      break;
  }

  // Установка частоты сигнала
  MDR_TIMER1->ARR = FRQ_TO_CNT(signal->frequency, DAC_SRM);

  // Запуск таймера
  MDR_TIMER1->CNTRL |= TIMER_CNTRL_CNT_EN;

  // Отображение параметров сигнала
  DAC_PrintBrief(signal);
}

// Верификация параметров сигнала
void DAC_AssertWaveform(signal_t *signal)
{
  // Нижнее значение константы
  if (signal->constant < 0.00) {
    signal->constant = 0.00;
  }

  // Верхнее значение константы
  else if (signal->constant > DAC_REF) {
    signal->constant = DAC_REF;
  }

  // Нижнее значение амплитуды
  if (signal->amplitude < 0.00) {
    signal->amplitude = 0.00;
  }

  // Верхняя граница напряжения
  if (signal->amplitude + signal->constant > DAC_REF) {
    signal->amplitude = DAC_REF - signal->constant;
  }

  // Нижняя граница напряжения
  if (signal->amplitude > signal->constant) {
    signal->amplitude = signal->constant;
  }

  // Верхняя граница частоты
  if (signal->frequency > SystemCoreClock / ((TIMER_ARR_MIN + 1) * TIMER_PSG * DAC_SRM)) {
    signal->frequency = SystemCoreClock / ((TIMER_ARR_MIN + 1) * TIMER_PSG * DAC_SRM);
  }

  // Нижняя граница частоты
  else if (signal->frequency < SystemCoreClock / ((TIMER_ARR_MAX + 1) * TIMER_PSG * DAC_SRM)) {
    signal->frequency = SystemCoreClock / ((TIMER_ARR_MAX + 1) * TIMER_PSG * DAC_SRM);
  }
}

// Отображение параметров сигнала
void DAC_PrintBrief(signal_t *signal)
{
  LCD_PrintString("    C = %.2f V", 3, signal->constant);   // Амплитуда (в вольтах)
  LCD_PrintString("    A = %.2f V", 4, signal->amplitude);  // Константа (в вольтах)
  LCD_PrintString("    F = %u Hz",  5, signal->frequency);  // Частота (в герцах)
  LCD_PrintString("    M = %u",     6, DAC_SRM);            // Коэф. дискретизации
}
