﻿/******************************************************************************
 * @File: dma.h
 * @Author: Milandr, L.
 * @Project: #07.3_ADC_[Multichannel_Conversion]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 27.03.2020
 * @Purpose: Конфигурация прямого доступа к памяти
 ******************************************************************************/

// Условная компиляция
#ifndef __DMA_H
#define __DMA_H

// Подключение заголовков модулей проекта
#include "link.h"

// Прототипы функций
void DMA_Reset(void);
void DMA_Init(void);

#endif
