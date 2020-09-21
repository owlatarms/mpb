/******************************************************************************
 * @File: dma.h
 * @Author: Milandr, L.
 * @Project: #06.2_DAC_[Cyclic_Conversion]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 19.05.2020
 * @Purpose: Конфигурация прямого доступа к памяти
 ******************************************************************************/

// Условная компиляция
#ifndef __DMA_H
#define __DMA_H

// Подключение заголовков
#include "link.h"

// Прототипы функций
void DMA_Reset(void);
void DMA_Init(void);

#endif
