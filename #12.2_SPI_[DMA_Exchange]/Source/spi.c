/******************************************************************************
 * @File: spi.c
 * @Author: Milandr, L.
 * @Project: #12.2_SPI_[DMA_Exchange]
 * @Version: 1.0.0
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 06.08.2020
 * @Purpose: Конфигурация последовательного периферийного интерфейса
 ******************************************************************************/

// Подключение заголовка
#include "spi.h"

// Инициализация глобальных переменных
// Буферы приёма и передачи
volatile uint8_t SPI_RX_BUF[SPI_BUF_SIZE] = {0};
volatile uint8_t SPI_TX_BUF[SPI_BUF_SIZE] = {0};

// Флаги завершения циклов обмена
volatile bool flg_spi_rx_eoc = false;
volatile bool flg_spi_tx_eoc = false;

// Мьютекс интерфейса
bool spi_mutex = true;

// Деинициализация SPI
void SPI_Reset(MDR_SSP_TypeDef* MDR_SSPx)
{
  MDR_SSPx->CR0   = 0;
  MDR_SSPx->CR1   = 0;
  MDR_SSPx->CPSR  = 0;
  MDR_SSPx->IMSC  = 0;
  MDR_SSPx->DMACR = 0;
}

// Инициализация SPI
void SPI_Init(void)
{
  // Включение тактирования порта D и SPI2
  MDR_RST_CLK->PER_CLOCK  |= RST_CLK_PCLK_PORTD_Msk
                           | RST_CLK_PCLK_SSP2_Msk;
  MDR_RST_CLK->SSP_CLOCK |= RST_CLK_SSP_CLOCK_SSP2_CLK_EN;

  // Конфигурация линий PD2 (MISO), PD5 (SCK), PD6 (MOSI), PD7 (CS)
  MDR_PORTD->OE     |=  0x000000E0;  // Направление (вывод; PD2 - ввод)
  MDR_PORTD->FUNC   &= ~0x0000FC30;  // Сброс битов регистра
  MDR_PORTD->FUNC   |=  0x00002820;  // Функция (альтернативная; PD7 - ввод-вывод)
  MDR_PORTD->ANALOG |=  0x000000E4;  // Режим (цифровой)
  MDR_PORTD->PULL   &= ~0x00E400E4;  // Подтяжка (отключена)
  MDR_PORTD->PD     &= ~0x00E400E4;  // Управление (не используется)
  MDR_PORTD->PWR    |=  0x0000FC30;  // Крутизна (высокая)
  MDR_PORTD->GFEN   &= ~0x000000E4;  // Фильтр (не используются)

  // Деинициализация
  SPI_Reset(MDR_SSP2);

  // Конфигурация контроллера
  MDR_SSP2->CR0 = (7 << SSP_CR0_DSS_Pos)   // Длина пакета данных (8 бит)
                | (0 << SSP_CR0_FRF_Pos)   // Формат пакета данных (SPI)
                | (0 << SSP_CR0_SPO_Pos)   // Полярность тактовых импульсов (CPOL = 0)
                | (0 << SSP_CR0_SPH_Pos)   // Фронт выборки данных (передний: CPHA = 0)
                | (0 << SSP_CR0_SCR_Pos);  // Фактор скорости передачи данных (не используется)

  MDR_SSP2->CR1 = (0 << SSP_CR1_LBM_Pos)   // Режим самотестирования (отключён)
                | (1 << SSP_CR1_SSE_Pos)   // Работа приёмопередатчика (включён)
                | (0 << SSP_CR1_MS_Pos)    // Роль в сети (ведущий)
                | (0 << SSP_CR1_SOD_Pos);  // Отключение линии передачи (не используется)

  // Установка делителя тактовой частоты
  MDR_SSP2->CPSR = SystemCoreClock / SPI_SPEED;

  // Исходное (неактивное) состояние линии CS
  SPI_CS_HIGH;

  // Инициализация DMA
  DMA_Init();
}

// Базовый обмен данными
uint8_t SPI_RW(uint8_t data)
{
  // Передача данных
  MDR_SSP2->DR = data;

  // Ожидание завершения обмена
  while ((MDR_SSP2->SR & SSP_SR_BSY) != false);

  // Возврат содержимого приёмника
  return (uint8_t)MDR_SSP2->DR;
}

// Приём данных (DMA)
spi_status_t SPI_Receive(volatile uint8_t *data, size_t size)
{
  // Состояние конечного автомата
  static spi_fsm_t fsm_state = SPI_FSM_RDY;

  // Состояние «Интерфейс готов»
  if (fsm_state == SPI_FSM_RDY && spi_mutex == true) {

    // Захват мьютекса
    spi_mutex = false;

    // Конфигурация DMA
    // Адрес источника данных
    DMA_CONTROL_DATA[6].DMA_SRC_DATA_END_PTR = (uint32_t)&data[size - 1];
    DMA_CONTROL_DATA[7].DMA_DST_DATA_END_PTR = (uint32_t)&data[size - 1];

    // Количество передач
    DMA_CONTROL_DATA[6].DMA_CHANNEL_CFG |= ((size - 1) << DMA_CHANNEL_CFG_N_MINUS_1_Pos);
    DMA_CONTROL_DATA[7].DMA_CHANNEL_CFG |= ((size - 1) << DMA_CHANNEL_CFG_N_MINUS_1_Pos);

    // Включение запросов от SPI к DMA на приём и передачу
    MDR_SSP2->DMACR |= SSP_DMACR_RXDMAE | SSP_DMACR_TXDMAE;

    // Переход к состоянию «Интерфейс занят»
    fsm_state = SPI_FSM_BSY;

    // Выход с флагом «Приём инициирован»
    return SPI_TRX_INITIATED;
  }

  // Состояние «Интерфейс занят»
  if (fsm_state == SPI_FSM_BSY) {

    // Если цикл передач завершился...
    if (flg_spi_rx_eoc == true) {

      // Сброс флага
      flg_spi_rx_eoc = false;

      // Ожидание завершения обмена
      while ((MDR_SSP2->SR & SSP_SR_BSY) != false);

      // Переход к состоянию «Интерфейс готов»
      fsm_state = SPI_FSM_RDY;

      // Освобождение мьютекса
      spi_mutex = true;

      // Выход с флагом «Приём завершён»
      return SPI_TRX_COMPLETED;
    }
  }

  // Выход с флагом «Приём в процессе»
  return SPI_TRX_INPROCESS;
}

// Передача данных (DMA)
spi_status_t SPI_Transmit(volatile uint8_t *data, size_t size)
{
  // Состояние конечного автомата
  static spi_fsm_t fsm_state = SPI_FSM_RDY;

  // Состояние «Интерфейс готов»
  if (fsm_state == SPI_FSM_RDY && spi_mutex == true) {

    // Захват мьютекса
    spi_mutex = false;

    // Конфигурация DMA
    // Адрес источника данных
    DMA_CONTROL_DATA[6].DMA_SRC_DATA_END_PTR = (uint32_t)&data[size - 1];

    // Количество передач
    DMA_CONTROL_DATA[6].DMA_CHANNEL_CFG |= ((size - 1) << DMA_CHANNEL_CFG_N_MINUS_1_Pos);

    // Запрос от SPI к DMA на передачу
    MDR_SSP2->DMACR |= SSP_DMACR_TXDMAE;

    // Переход к состоянию «Занят»
    fsm_state = SPI_FSM_BSY;

    // Выход с флагом «Передача инициирована»
    return SPI_TRX_INITIATED;
  }

  // Состояние «Интерфейс занят»
  if (fsm_state == SPI_FSM_BSY) {

    // Если цикл передач завершился...
    if (flg_spi_tx_eoc == true) {

      // Сброс флага
      flg_spi_tx_eoc = false;

      // Ожидание завершения обмена
      while ((MDR_SSP2->SR & SSP_SR_BSY) != false);

      // Очистка буфера приёмника
      while ((MDR_SSP2->SR & SSP_SR_RNE) != false) {
        MDR_SSP2->DR;
      }

      // Переход к состоянию «Готов»
      fsm_state = SPI_FSM_RDY;

      // Освобождение мьютекса
      spi_mutex = true;

      // Выход с флагом «Передача завершена»
      return SPI_TRX_COMPLETED;
    }
  }

  // Выход с флагом «Передача в процессе»
  return SPI_TRX_INPROCESS;
}
