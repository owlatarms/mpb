/******************************************************************************
 * @File: lib_lcd.h
 * @Author: Milandr, L.
 * @Project: Library
 * @Version: 1.0.2
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 16.03.2020
 * @Purpose: Библиотека для работы с дисплеем MT-12864B (драйвер NT75451)
 ******************************************************************************/

// Условная компиляция
#ifndef __LIB_LCD_H
#define __LIB_LCD_H

// Подключение стандартных библиотек
#include <stdarg.h>    // Библиотека аргументов
#include <stdio.h>     // Библиотека ввода-вывода
#include <string.h>    // Библиотека строк
#include <1986VE9x.h>  // Библиотека микроконтроллера
#include <lib_clk.h>   // Конфигурация системы тактирования
#include <lib_font.h>  // Кодировка символов

// Скорость обмена данными с дисплеем (4 Мбит/с)
#define LCD_SPI_SPEED 4000000

// Ширина дисплея (в символах)
#define LCD_SCREEN_WIDTH 21

// Макросы работы с линией A0 (PE1)
#define LCD_MODE_CMD  (MDR_PORTE->RXTX &= ~PORT_RXTX1_Msk)
#define LCD_MODE_DATA (MDR_PORTE->RXTX |=  PORT_RXTX1_Msk)

// Макросы работы с подсветкой дисплея (PE2)
#define LCD_LED_OFF   (MDR_PORTE->RXTX &= ~PORT_RXTX2_Msk)
#define LCD_LED_ON    (MDR_PORTE->RXTX |=  PORT_RXTX2_Msk)

// Конфигурационная таблица
static const uint8_t lcd_cfg_table[] =
{
  0xAE, // Отключение дисплея                (команда 1)
  0xE2, // Сброс внутреннего состояния       (команда 14)
  0xA3, // Корректировка напряжения          (команда 11)
  0xA6, // Инверсия дисплея                  (команда 9)
  0xC8, // Зеркальное отображение, вертикаль (команда 15)
  0x81, // Контрастность дисплея, режим      (команда 18)
  0x20, // Контрастность дисплея, значение   (команда 18)
  0x2F, // Источник питания                  (команда 16)
  0x26, // Регулировка питания               (команда 17)
  0xAD, // Статический индикатор, режим      (команда 19)
  0x02, // Статический индикатор, значение   (команда 19)
  0xAF  // Включение дисплея                 (команда 1)
};

// Прототипы функций
void LCD_Init(void);
void LCD_WriteCmd(uint8_t cmd);
void LCD_WriteData(uint8_t data);
void LCD_SetPage(uint8_t page);
void LCD_SetColumn(uint8_t column);
void LCD_SetPointer(uint8_t x, uint8_t y);
void LCD_Inverse(bool state);
void LCD_ClearString(uint8_t line);
void LCD_ClearScreen(void);
void LCD_PrintSymbol(uint32_t symbol, uint8_t x, uint8_t y); 
void LCD_PrintString(const char *string, uint8_t y, ...);
void LCD_PrintLogo(void);

#endif
