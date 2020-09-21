/******************************************************************************
 * @File: spi.h
 * @Author: Milandr, L.
 * @Project: #12.1_SPI_[Basic_Exchange]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 26.08.2020
 * @Purpose: Конфигурация последовательного периферийного интерфейса
 ******************************************************************************/

// Условная компиляция
#ifndef __SPI_H
#define __SPI_H

// Подключение заголовков
#include "link.h"

// Скорость обмена данными по SPI (бит/с)
#define SPI_SPEED 1000000

// Макросы работы с линией CS (PD7)
#define SPI_CS_HIGH MDR_PORTD->RXTX |= PORT_RXTX7_Msk;
#define SPI_CS_LOW  MDR_PORTD->RXTX &= ~PORT_RXTX7_Msk;

// Прототипы функций
void SPI_Reset(MDR_SSP_TypeDef* MDR_SSPx);
void SPI_Init(void);
uint8_t SPI_RW(uint8_t data);

#endif
