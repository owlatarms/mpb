/*******************************************************************************
 * @File: can.c
 * @Author: Milandr, L.
 * @Project: #15.1_CAN_[Simple_Exchange]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 07.05.2020
 * @Purpose: Конфигурация контроллера шины CAN
 *******************************************************************************/
 
// Подключение заголовка
#include "can.h"

// Флаг приёма кадра
volatile bool flg_frm_rcvd = false;

// Деинициализация CAN
void CAN_Reset(MDR_CAN_TypeDef *MDR_CANx)
{
  MDR_CANx->CONTROL = 0;
  MDR_CANx->STATUS  = 0;
  MDR_CANx->BITTMNG = 0;
  MDR_CANx->INT_EN  = 0;
  MDR_CANx->OVER    = 0;
  MDR_CANx->INT_RX  = 0;
  MDR_CANx->INT_TX  = 0;

  // Сброс регистров управления буферами
  for (uint8_t i = 0; i < 32; i++) {
    MDR_CANx->BUF_CON[i] = 0;
  }

  // Сброс фильтров
  for (uint8_t i = 0; i < 32; i++) {
    MDR_CANx->CAN_BUF_FILTER[i].FILTER = 0;
    MDR_CANx->CAN_BUF_FILTER[i].MASK   = 0;
  }
}

// Инициализация CAN
void CAN_Init(void)
{
  // Включение тактирования порта A и CAN
  MDR_RST_CLK->PER_CLOCK |= RST_CLK_PCLK_PORTA_Msk
                          | RST_CLK_PCLK_CAN1_Msk;
  MDR_RST_CLK->CAN_CLOCK |= RST_CLK_CAN_CLOCK_CAN1_CLK_EN;

  // Конфигурация линий PA6 (TX) и PA7 (RX)
  MDR_PORTA->OE     |=  0x00000040;  // Направление (PA6 - вывод, PA7 - ввод)
  MDR_PORTA->FUNC   &= ~0x0000F000;  // Сброс битов регистра
  MDR_PORTA->FUNC   |=  0x0000A000;  // Функция (альтернативная)
  MDR_PORTA->ANALOG |=  0x000000C0;  // Режим (цифровой)
  MDR_PORTA->PULL   &= ~0x00C000C0;  // Подтяжка (отключена)
  MDR_PORTA->PD     &= ~0x00C000C0;  // Управление (не используется)
  MDR_PORTA->PWR    |=  0x0000F000;  // Крутизна (высокая)
  MDR_PORTA->GFEN   &= ~0x000000C0;  // Фильтр (не используются)

  // Деинициализация
  CAN_Reset(MDR_CAN1);

  // Конфигурация контроллера
  MDR_CAN1->CONTROL = (1 << CAN_CONTROL_CAN_EN_Pos)  // Работа контроллера (включён)
                    | (0 << CAN_CONTROL_ROM_Pos)     // Режим «только чтение» (отключён)
                    | (1 << CAN_CONTROL_STM_Pos)     // Режим самотестирования (включён)
                    | (1 << CAN_CONTROL_SAP_Pos)     // Подтв. приёма собственных кадров (включено)
                    | (1 << CAN_CONTROL_ROP_Pos);    // Приём собственных кадров (включён)

  // Конфигурация битового интервала
  MDR_CAN1->BITTMNG = (15 << CAN_BITTMNG_BRP_Pos)  // Делитель тактовой частоты (16 – 1)
                    | (1 << CAN_BITTMNG_PSEG_Pos)  // Сегмент распространения (2 TQ)
                    | (3 << CAN_BITTMNG_SEG1_Pos)  // Фазовый сегмент 1 (4 TQ)
                    | (2 << CAN_BITTMNG_SEG2_Pos)  // Фазовый сегмент 2 (3 TQ)
                    | (1 << CAN_BITTMNG_SJW_Pos)   // Подстройка сегментов (2 TQ)
                    | (0 << CAN_BITTMNG_SB_Pos);   // Выборка (однократная)

  // Конфигурация буферов
  // Буфер приёма
  MDR_CAN1->BUF_CON[RX_BUF] = (1 << CAN_BUF_CON_EN_Pos)        // Работа буфера (включён)
                            | (1 << CAN_BUF_CON_RX_TXN_Pos)    // Режим работы буфера (приём)
                            | (0 << CAN_BUF_CON_OVER_EN_Pos)   // Перезапись кадра (запрещена)
                            | (0 << CAN_BUF_CON_RTR_EN_Pos)    // Ответ на удаленный запрос (не используется)
                            | (1 << CAN_BUF_CON_PRIOR_0_Pos);  // Приоритет буфера (обычный)

  // Буфер передачи
  MDR_CAN1->BUF_CON[TX_BUF] = (1 << CAN_BUF_CON_EN_Pos)        // Работа буфера (включён)
                            | (0 << CAN_BUF_CON_RX_TXN_Pos)    // Режим работы буфера (передача)
                            | (0 << CAN_BUF_CON_OVER_EN_Pos)   // Перезапись кадра (не используется)
                            | (0 << CAN_BUF_CON_RTR_EN_Pos)    // Ответ на удаленный запрос (запрещён)
                            | (1 << CAN_BUF_CON_PRIOR_0_Pos);  // Приоритет буфера (обычный)

  // Настройка запросов на обработку прерываний
  MDR_CAN1->INT_EN = CAN_INT_EN_GLB_INT_EN  // Общий флаг запросов
                   | CAN_INT_EN_RX_INT_EN;  // Прерывание при приеме кадра

  // Установка прерывания при приёме кадра в буфер
  MDR_CAN1->INT_RX = (1 << RX_BUF);

  // Назначение приоритета прерывания
  NVIC_SetPriority(CAN1_IRQn, 6);

  // Разрешение обработки прерывания
  NVIC_EnableIRQ(CAN1_IRQn);
}

// Передача кадра
void CAN_SendFrame(frame_t frame, uint8_t buf_number)
{
  // Вычисление длины кадра
  frame.dlc = 8;

  // Цикл проверки старших байтов
  for (uint8_t i = 0; i < 4; i++) {
    if ((frame.datah & (CAN_DATAH_DB7_Msk >> (i * 8))) == 0) {
      frame.dlc--;
    } else {
      break;
    }
  }

  // Цикл проверки младших байтов
  for (uint8_t i = 0; i < 4; i++) {
    if (frame.dlc > 4) {
      break;
    } else if ((frame.datal & (CAN_DATAL_DB3_Msk >> (i * 8))) == 0) {
      frame.dlc--;
    } else {
      break;
    }
  }

  // Формирование кадра
  MDR_CAN1->CAN_BUF[buf_number].ID    = (frame.id  << CAN_ID_EID_Pos);  // Идентификатор кадра
  MDR_CAN1->CAN_BUF[buf_number].DLC   = (frame.dlc << CAN_DLC_Pos)      // Биты DLC (1 байт)
                                      | (0 << CAN_DLC_RTR_Pos)          // Бит RTR (нет запроса)
                                      | (1 << CAN_DLC_R1_Pos)           // Бит R1 (всегда должен быть равен 1)
                                      | (0 << CAN_DLC_R0_Pos)           // Бит R0 (всегда должен быть равен 0)
                                      | (1 << CAN_DLC_SSR_Pos)          // Бит SSR (всегда должен быть равен 1)
                                      | (1 << CAN_DLC_IDE_Pos);         // Бит IDE (расширенный формат)
  MDR_CAN1->CAN_BUF[buf_number].DATAL = frame.datal;                    // Байты данных 1...4
  MDR_CAN1->CAN_BUF[buf_number].DATAH = frame.datah;                    // Байты данных 5...8

  // Запрос на передачу кадра
  MDR_CAN1->BUF_CON[buf_number] |= CAN_BUF_CON_TX_REQ;
}

// Извлечение кадра из буфера
frame_t CAN_ExtractFrame(uint8_t buf_number)
{
  // Структура кадра
  frame_t frame;

  // Извлечение кадра из буфера
  frame.id    = MDR_CAN1->CAN_BUF[buf_number].ID;
  frame.dlc   = MDR_CAN1->CAN_BUF[buf_number].DLC & 0xF;
  frame.datal = MDR_CAN1->CAN_BUF[buf_number].DATAL;
  frame.datah = MDR_CAN1->CAN_BUF[buf_number].DATAH;

  // Возврат заполненной структуры
  return frame;
}

// Отображение содержания кадра
void CAN_PrintFrame(frame_t frame)
{
  LCD_PrintString("    ID  = %u", 3, frame.id);
  LCD_PrintString("    DLC = %u", 4, frame.dlc);
  LCD_PrintString("    DTL = %u", 5, frame.datal);
  LCD_PrintString("    DTH = %u", 6, frame.datah);
}

// Обработчик прерываний от контроллера CAN1
void CAN1_IRQHandler(void)
{
  // Сброс флага прерывания
  MDR_CAN1->BUF_CON[RX_BUF] &= ~CAN_BUF_CON_RX_FULL;

  // Установка флага приёма кадра
  flg_frm_rcvd = true;
}
