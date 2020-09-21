/******************************************************************************
 * @File: eeprom.h
 * @Author: Milandr, L.
 * @Project: #12.2_SPI_[DMA_Exchange]
 * @Version: 1.0.0
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 06.08.2020
 * @Purpose: Функции работы с микросхемой памяти 25LCXXX (Microchip)
 ******************************************************************************/

// Условная компиляция
#ifndef __EEPROM_H
#define __EEPROM_H

// Подключение заголовков
#include "link.h"

// Константы EEPROM 25LCXXX
// Размер памяти (в байтах)
#define EEPROM_DENSITY 128 * 1024

// Размер страницы (в байтах)
#define EEPROM_PAGE_SIZE 64

// Инструкции
#define EEPROM_READ  0x03  // Чтение данных
#define EEPROM_WRITE 0x02  // Запись данных
#define EEPROM_WRDI  0x04  // Запрет записи данных
#define EEPROM_WREN  0x06  // Разрешение записи данных
#define EEPROM_RDSR  0x05  // Чтение регистра состояний
#define EEPROM_WRSR  0x01  // Запись регистра состояний

// Состояния
#define EEPROM_WIP   0x01  // Запись данных в процессе
#define EEPROM_WEL   0x02  // Разрешение записи
#define EEPROM_BP0   0x04  // Блок защиты памяти 0
#define EEPROM_BP1   0x08  // Блок защиты памяти 1

// Прототипы функций
uint8_t EEPROM_ReadStatus(void);
void EEPROM_WriteEnable(void);
void EEPROM_ReadData(uint32_t address, uint8_t *data, size_t size);
void EEPROM_WriteData(uint32_t address, uint8_t *data, size_t size);
void EEPROM_ErasePage(uint16_t page);
void EEPROM_EraseAll(void);

#endif
