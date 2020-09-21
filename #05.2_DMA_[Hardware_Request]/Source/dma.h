/******************************************************************************
 * @File: dma.h
 * @Author: Milandr, L.
 * @Project: #05.2_DMA_[Hardware_Request]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 07.05.2020
 * @Purpose: Конфигурация прямого доступа к памяти
 ******************************************************************************/

// Условная компиляция
#ifndef __DMA_H
#define __DMA_H

// Подключение заголовков
#include "link.h"

// Флаги завершения циклов передач
#define DMA_EOC_ONE 0x01  // Первая половина
#define DMA_EOC_TWO 0x02  // Вторая половина

// Прототипы функций
void DMA_Reset(void);
void DMA_Init(void);

#endif
