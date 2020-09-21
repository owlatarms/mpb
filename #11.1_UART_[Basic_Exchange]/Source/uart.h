/******************************************************************************
 * @File: uart.h
 * @Author: Milandr, L.
 * @Project: #11.1_UART_[Simple_Exchange]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 29.06.2020
 * @Purpose: Конфигурация универсального асинхронного приёмопередатчика
 ******************************************************************************/

// Условная компиляция
#ifndef __UART_H
#define __UART_H

// Подключение заголовков
#include "link.h"

// Скорость обмена данными по UART (бод)
#define UART_SPEED 115200

// Кроссмодульные переменные
extern uint32_t parity_error;

// Прототипы функций
void UART_Reset(MDR_UART_TypeDef *MDR_UARTx);
void UART_Init(void);

#endif
