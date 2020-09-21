/******************************************************************************
 * @File: spi.c
 * @Author: Milandr, L.
 * @Project: #12.1_SPI_[Basic_Exchange]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 26.08.2020
 * @Purpose: Конфигурация последовательного периферийного интерфейса
 ******************************************************************************/

// Подключение заголовка
#include "spi.h"

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
