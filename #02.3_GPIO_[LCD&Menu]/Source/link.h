/******************************************************************************
 * @File: link.h
 * @Author: Milandr, L.
 * @Project: #02.3_GPIO_[LCD&Menu]
 * @Version: 1.1.0
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 15.10.2020
 * @Purpose: Связывание модулей проекта
 ******************************************************************************/

// Условная компиляция
#ifndef __LINK_H
#define __LINK_H

// Подключение библиотеки микроконтроллера
#include <1986VE9x.h>

// Подключение заголовков модулей проекта
#include <lib_btn.h>
#include <lib_lcd.h>
#include <lib_menu.h>
#include "main.h"
#include "led.h"

#endif
