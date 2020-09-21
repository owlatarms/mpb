/******************************************************************************
 * @File: dma.h
 * @Author: Milandr, L.
 * @Project: #11.2_UART_[Exchange_Via_DMA]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 30.06.2020
 * @Purpose: Конфигурация прямого доступа к памяти
 ******************************************************************************/

// Условная компиляция
#ifndef __DMA_H
#define __DMA_H

// Подключение заголовков
#include "link.h"

// Кроссмодульные переменные
extern volatile DMA_ControlDataTypeDef DMA_CONTROL_DATA[64];

// Прототипы функций
void DMA_Reset(void);
void DMA_Init(void);

#endif
