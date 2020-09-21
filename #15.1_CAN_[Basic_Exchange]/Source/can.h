/*******************************************************************************
 * @File: can.h
 * @Author: Milandr, L.
 * @Project: #15.1_CAN_[Simple_Exchange]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 07.05.2020
 * @Purpose: Конфигурация контроллера шины CAN
 *******************************************************************************/

// Условная компиляция
#ifndef __CAN_H
#define __CAN_H

// Подключение заголовков
#include "link.h"

// Структура кадра
typedef struct
{
  uint32_t id;     // Идентификатор
  uint8_t  dlc;    // Код длины данных
  uint32_t datal;  // Байты данных 1...4
  uint32_t datah;  // Байты данных 5...8
} frame_t;

// Псевдонимы буферов CAN
#define RX_BUF 0  // Буфер приёма
#define TX_BUF 1  // Буфер передачи

// Кроссмодульные переменные
extern volatile bool flg_frm_rcvd;

// Прототипы функций
void CAN_Reset(MDR_CAN_TypeDef *MDR_CANx);
void CAN_Init(void);
void CAN_SendFrame(frame_t frame, uint8_t buf_number);
frame_t CAN_ExtractFrame(uint8_t buffer_number);
void CAN_PrintFrame(frame_t frame);

#endif
