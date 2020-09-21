/******************************************************************************
 * @File: uart.h
 * @Author: Milandr, L.
 * @Project: #11.2_UART_[Exchange_Via_DMA]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 30.06.2020
 * @Purpose: Конфигурация универсального асинхронного приёмопередатчика
 ******************************************************************************/

// Условная компиляция
#ifndef __UART_H
#define __UART_H

// Подключение заголовков
#include "link.h"

// Скорость обмена данными по UART (бод)
#define UART_SPEED 115200

// Размер буфера для приёмника и передатчика
#define UART_RX_BUF_SIZE (LCD_SCREEN_WIDTH * 4)
#define UART_TX_BUF_SIZE 128

// Псевдонимы состояний интерфейса
typedef enum {
  UART_TRX_INPROCESS = 0,  // «Передача в процессе»
  UART_TRX_INITIATED = 1,  // «Передача инициирована»
  UART_TRX_COMPLETED = 2   // «Передача завершена»
} uart_status_t;

// Псевдонимы состояний конечного автомата
typedef enum {
  UART_FSM_RDY = 0,  // «Интерфейс готов»
  UART_FSM_BSY = 1,  // «Интерфейс занят»
} uart_fsm_t;

// Кроссмодульные переменные
extern volatile uint8_t UART_RX_BUF[UART_RX_BUF_SIZE + 1];
extern volatile uint8_t UART_TX_BUF[UART_TX_BUF_SIZE];
extern volatile bool flg_uart_rx_upd;
extern volatile bool flg_uart_tx_eoc;

// Прототипы функций
void UART_Reset(MDR_UART_TypeDef *MDR_UARTx);
void UART_Init(void);
uart_status_t UART_Transmit(volatile uint8_t *data, size_t size);

#endif
