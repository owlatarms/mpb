/******************************************************************************
 * @File: spi.h
 * @Author: Milandr, L.
 * @Project: #12.2_SPI_[DMA_Exchange]
 * @Version: 1.0.0
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 06.08.2020
 * @Purpose: Конфигурация последовательного периферийного интерфейса
 ******************************************************************************/

// Условная компиляция
#ifndef __SPI_H
#define __SPI_H

// Подключение заголовков
#include "link.h"

// Скорость обмена данными по SPI (бит/с)
#define SPI_SPEED 1000000

// Размер буфера для приёмника и передатчика
#define SPI_BUF_SIZE 128

// Макросы работы с линией CS (PD7)
#define SPI_CS_HIGH MDR_PORTD->RXTX |= PORT_RXTX7_Msk;
#define SPI_CS_LOW  MDR_PORTD->RXTX &= ~PORT_RXTX7_Msk;

// Псевдонимы состояний интерфейса
typedef enum {
  SPI_TRX_INPROCESS = 0,  // «Передача в процессе»
  SPI_TRX_INITIATED = 1,  // «Передача инициирована»
  SPI_TRX_COMPLETED = 2   // «Передача завершена»
} spi_status_t;

// Псевдонимы состояний конечного автомата
typedef enum {
  SPI_FSM_RDY = 0,  // «Интерфейс готов»
  SPI_FSM_BSY = 1,  // «Интерфейс занят»
} spi_fsm_t;

// Кроссмодульные переменные
extern volatile uint8_t SPI_RX_BUF[SPI_BUF_SIZE];
extern volatile uint8_t SPI_TX_BUF[SPI_BUF_SIZE];
extern volatile bool flg_spi_rx_eoc;
extern volatile bool flg_spi_tx_eoc;

// Прототипы функций
void SPI_Reset(MDR_SSP_TypeDef* MDR_SSPx);
void SPI_Init(void);
uint8_t SPI_RW(uint8_t data);
spi_status_t SPI_Receive(volatile uint8_t *data, size_t size);
spi_status_t SPI_Transmit(volatile uint8_t *data, size_t size);

#endif
