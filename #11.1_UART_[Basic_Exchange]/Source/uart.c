/******************************************************************************
 * @File: uart.c
 * @Author: Milandr, L.
 * @Project: #11.1_UART_[Simple_Exchange]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 29.06.2020
 * @Purpose: Конфигурация универсального асинхронного приёмопередатчика
 ******************************************************************************/

// Подключение заголовка
#include "uart.h"

// Счётчик ошибок контроля чётности
uint32_t parity_error = 0;

// Деинициализация UART
void UART_Reset(MDR_UART_TypeDef *MDR_UARTx)
{
  MDR_UARTx->RSR_ECR = 0;
  MDR_UARTx->FR      = UART_FR_RXFE   // Сброс всех флагов, кроме флагов
                     | UART_FR_TXFE;  // отсутствия данных в приёмнике и передатчике
  MDR_UARTx->ILPR    = 0;
  MDR_UARTx->IBRD    = 0;
  MDR_UARTx->FBRD    = 0;
  MDR_UARTx->LCR_H   = 0;
  MDR_UARTx->CR      = 0;
  MDR_UARTx->IFLS    = 0;
  MDR_UARTx->IMSC    = 0;
  MDR_UARTx->DMACR   = 0;
}

// Инициализация UART (2)
void UART_Init(void)
{
  // Включение тактирования порта D и UART2
  MDR_RST_CLK->PER_CLOCK  |= RST_CLK_PCLK_PORTD_Msk
                           | RST_CLK_PCLK_UART2_Msk;
  MDR_RST_CLK->UART_CLOCK |= RST_CLK_UART_CLOCK_UART2_CLK_EN;

  // Конфигурация линий PD0 (RX) и PD1 (TX)
  MDR_PORTD->OE     |=  0x00000003;  // Направление (не используется)
  MDR_PORTD->FUNC   &= ~0x0000000F;  // Сброс битов регистра
  MDR_PORTD->FUNC   |=  0x0000000A;  // Функция (альтернативная)
  MDR_PORTD->ANALOG |=  0x00000003;  // Режим (цифровой)
  MDR_PORTD->PULL   &= ~0x00030003;  // Подтяжка (отключена)
  MDR_PORTD->PD     &= ~0x00030003;  // Управление (не используется)
  MDR_PORTD->PWR    |=  0x0000000F;  // Крутизна (высокая)
  MDR_PORTD->GFEN   &= ~0x00000003;  // Фильтр (не используются)

  // Деинициализация UART
  UART_Reset(MDR_UART2);

  // Установка скорости обмена данными (задаётся константой)
  // Вычисление требуемого делителя тактовой частоты
  uint32_t scaler  = SystemCoreClock / (UART_SPEED / 4);

  // Запись целой части делителя (IBRD)
  MDR_UART2->IBRD = scaler >> 6;

  // Запись дробной части делителя (FBRD)
  MDR_UART2->FBRD = scaler & 0x3F;

  // Конфигурация формата данных
  MDR_UART2->LCR_H = (1 << UART_LCR_H_PEN_Pos)   // Контрольный бит (включён)
                   | (1 << UART_LCR_H_EPS_Pos)   // Чётность или нечётность (чётность)
                   | (0 << UART_LCR_H_STP2_Pos)  // Количество STOP-битов (один)
                   | (1 << UART_LCR_H_FEN_Pos)   // Буфер FIFO (включён)
                   | (3 << UART_LCR_H_WLEN_Pos)  // Длина пакета данных (8 бит)
                   | (0 << UART_LCR_H_SPS_Pos);  // Метка или пробел (не используется)

  // Конфигурация контроллера UART
  MDR_UART2->CR = (1 << UART_CR_UARTEN_Pos)  // Работа контроллера UART (включён)
                | (0 << UART_CR_LBE_Pos)     // Режим самотестирования (отключён)
                | (1 << UART_CR_TXE_Pos)     // Разрешение передачи (разрешена)
                | (1 << UART_CR_RXE_Pos);    // Разрешение приёма (разрешён)

  // Настройка запросов на обработку прерываний
  MDR_UART2->IMSC = UART_IMSC_PEIM;  // Прерывание при ошибке контроля чётности

  // Конфигурация контроллера NVIC
  NVIC_SetPriority(UART2_IRQn, 5);  // Приоритет прерываний
  NVIC_EnableIRQ(UART2_IRQn);       // Разрешение обработки прерываний
}

// Обработчик прерываний от контроллера UART2
void UART2_IRQHandler(void)
{
  // Сброс запроса на обработку прерывания
  // (осуществляется записью единицы)
  MDR_UART2->ICR |= UART_ICR_PEIC;

  // Инкремент счётчика ошибок контроля чётности
  parity_error++;
}
