/******************************************************************************
 * @File: lib_btn.c
 * @Author: Milandr, L.
 * @Project: Library
 * @Version: 1.2.0
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 24.03.2020
 * @Purpose: Библиотека для работы кнопок
 ******************************************************************************/

// Подключение заголовка
#include "lib_btn.h"

// Инициализация портов для работы кнопок
void BTN_Init(void)
{
  // Включение тактирования портов B и Е
  MDR_RST_CLK->PER_CLOCK |= RST_CLK_PCLK_PORTB_Msk
                          | RST_CLK_PCLK_PORTE_Msk;

  // Конфигурация линий PB4, PB5 и PB10 (кнопки DOWN, SENSE и LEFT)
  MDR_PORTB->OE     &= ~0x00000430;  // Направление (ввод)
  MDR_PORTB->FUNC   &= ~0x00300F00;  // Функция (ввод-вывод)
  MDR_PORTB->ANALOG |=  0x00000430;  // Режим (цифровой)
  MDR_PORTB->PULL   &= ~0x04300430;  // Подтяжка (отключена)
  MDR_PORTB->PD     &= ~0x04300430;  // Триггер Шмитта (отключён)
  MDR_PORTB->PWR    &= ~0x00300F00;  // Крутизна (не используется)
  MDR_PORTB->GFEN   &= ~0x00000430;  // Фильтр (отключён)

  // Конфигурация линий PE3, PE6 и PE7 (кнопки RIGHT, UP и MID)
  MDR_PORTE->OE     &= ~0x000000C8;  // Направление (ввод)
  MDR_PORTE->FUNC   &= ~0x0000F0C0;  // Функция (ввод-вывод)
  MDR_PORTE->ANALOG |=  0x000000C8;  // Режим (цифровой)
  MDR_PORTE->PULL   &= ~0x00C800C8;  // Подтяжка (отключена)
  MDR_PORTE->PD     &= ~0x00C800C8;  // Триггер Шмитта (отключён)
  MDR_PORTE->PWR    &= ~0x0000F0C0;  // Крутизна (не используется)
  MDR_PORTE->GFEN   &= ~0x000000C8;  // Фильтр (отключён)
}

// Обработчик кнопок
bool BTN_Handler(btn_alias_t button)
{
  // Состояния конечного автомата
  static btn_fsm_t fsm_state[BTN_NUM] = {BTN_FSM_PRESS};

  // Параметры кнопки
  uint32_t *port = 0;  // Адрес порта
  uint16_t  mask = 0;  // Маска бита
  uint8_t   pull = 0;  // Подтяжка (земля / питание)

  // Счётчик (защита от дребезга)
  int i = 0, counter = 0;

  // Определение параметров линии
       if (button == BTN_U) {port = (uint32_t *)&MDR_PORTE->RXTX; mask = 0x0040; pull = 1;}  // Кнопка UP (PE6)
  else if (button == BTN_D) {port = (uint32_t *)&MDR_PORTB->RXTX; mask = 0x0010; pull = 1;}  // Кнопка DOWN (PB4)
  else if (button == BTN_L) {port = (uint32_t *)&MDR_PORTB->RXTX; mask = 0x0400; pull = 1;}  // Кнопка LEFT (PB10)
  else if (button == BTN_R) {port = (uint32_t *)&MDR_PORTE->RXTX; mask = 0x0008; pull = 1;}  // Кнопка RIGHT (PE3)
  else if (button == BTN_M) {port = (uint32_t *)&MDR_PORTE->RXTX; mask = 0x0080; pull = 1;}  // Кнопка MID (PE7)
  else if (button == BTN_S) {port = (uint32_t *)&MDR_PORTB->RXTX; mask = 0x0020; pull = 0;}  // Кнопка SENSE (PB5)
  else                      {return false;}

  /* --- Машина состояний --- */
  switch (fsm_state[button]) {

    // Состояние «Ожидание нажатия»
    case BTN_FSM_PRESS:

      // Проверка нажатия кнопки
      if ((!!(*port & mask) ^ pull) == true) {

        // Защита от дребезга: счётчик
        for (i = 0, counter = 0; i < BTN_CNT_LIM; i++) {
          counter += !!(*port & mask) ^ pull;
        }

        // Проверка состояния счётчика
        if (counter == BTN_CNT_LIM) {           // Если счётчик достиг предела...
          fsm_state[button] = BTN_FSM_RELEASE;  // Переход к состоянию «Ожидание отпускания»
          return true;                          // Позитивный выход
        }
      }

      break;

    // Состояние «Ожидание отпускания»
    case BTN_FSM_RELEASE:

      // Проверка отпускания кнопки
      if ((!!(*port & mask) ^ pull) == false) {

        // Защита от дребезга: счётчик
        for (i = 0, counter = 0; i < BTN_CNT_LIM; i++) {
          counter += !(!!(*port & mask) ^ pull);
        }

        // Проверка состояния счётчика
        if (counter == BTN_CNT_LIM) {         // Если счётчик достиг предела...
          fsm_state[button] = BTN_FSM_PRESS;  // Переход к состоянию «Ожидание нажатия»
        }
      }

      break;
  }

  // Негативный выход
  return false;
}
