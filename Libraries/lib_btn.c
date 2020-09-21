/******************************************************************************
 * @File: lib_btn.c
 * @Author: Milandr, L.
 * @Project: Library
 * @Version: 1.1.0
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
  MDR_PORTB->ANALOG |=  0x00000430;  // Режим работы (цифровой)
  MDR_PORTB->PULL   &= ~0x04300430;  // Подтяжка (отключена)
  MDR_PORTB->PD     &= ~0x04300430;  // Триггер Шмитта (отключен)
  MDR_PORTB->PWR    &= ~0x00300F00;  // Крутизна фронтов (не используется)
  MDR_PORTB->GFEN   &= ~0x00000430;  // Цифровой фильтр (отключен)

  // Конфигурация линий PE3, PE6 и PE7 (кнопки RIGHT, UP и MID)
  MDR_PORTE->OE     &= ~0x000000C8;  // Направление (ввод)
  MDR_PORTE->FUNC   &= ~0x0000F0C0;  // Функция (ввод-вывод)
  MDR_PORTE->ANALOG |=  0x000000C8;  // Режим работы (цифровой)
  MDR_PORTE->PULL   &= ~0x00C800C8;  // Подтяжка (отключена)
  MDR_PORTE->PD     &= ~0x00C800C8;  // Триггер Шмитта (отключен)
  MDR_PORTE->PWR    &= ~0x0000F0C0;  // Крутизна фронтов (не используется)
  MDR_PORTE->GFEN   &= ~0x000000C8;  // Цифровой фильтр (отключен)
}

// Обработчик кнопок
bool BTN_Handler(btn_alias_t button)
{
  // Состояния конечного автомата
  static btn_fsm_t fsm_state[BTN_NUM] = {BTN_FSM_PRESS_CHECK};

  // Параметры кнопки
  uint32_t *port = 0;  // Адрес порта
  uint16_t  mask = 0;  // Маска бита
  uint8_t   pull = 0;  // Подтяжка (земля / питание)

  // Счётчик (защита от дребезга)
  uint32_t counter = 0;

  // Определение состояния линии
       if (button == BTN_U) {port = (uint32_t *)&MDR_PORTE->RXTX; mask = 0x0040; pull = 1;}  // Кнопка UP (PE6)
  else if (button == BTN_D) {port = (uint32_t *)&MDR_PORTB->RXTX; mask = 0x0010; pull = 1;}  // Кнопка DOWN (PB4)
  else if (button == BTN_L) {port = (uint32_t *)&MDR_PORTB->RXTX; mask = 0x0400; pull = 1;}  // Кнопка LEFT (PB10)
  else if (button == BTN_R) {port = (uint32_t *)&MDR_PORTE->RXTX; mask = 0x0008; pull = 1;}  // Кнопка RIGHT (PE3)
  else if (button == BTN_M) {port = (uint32_t *)&MDR_PORTE->RXTX; mask = 0x0080; pull = 1;}  // Кнопка MID (PE7)
  else if (button == BTN_S) {port = (uint32_t *)&MDR_PORTB->RXTX; mask = 0x0020; pull = 0;}  // Кнопка SENSE (PB5)

  /* --- Конечный автомат --- */
  // Состояние «Ожидание нажатия»
  if (fsm_state[button] == BTN_FSM_PRESS_CHECK) {

    // Проверка состояния кнопки
    if (((bool)(*port & mask) ^ pull) == true) {     // Если кнопка нажата...
      fsm_state[button] = BTN_FSM_PRESS_DEBOUNCE;    // Переход к следующему состоянию
    } else {                                         // Если кнопка не нажата...
      return false;                                  // Негативный выход
    }
  }

  // Состояние «Защита от дребезга при нажатии»
  if (fsm_state[button] == BTN_FSM_PRESS_DEBOUNCE) {

    // Сброс счётчика
    counter = 0;

    // Цикл счёта
    for (int i = 0; i < BTN_CNT_LIM; i++) {
      counter += (bool)(*port & mask) ^ pull;
    }

    // Проверка состояния счётчика
    if (counter == BTN_CNT_LIM) {                    // Если счётчик достиг предела...
      fsm_state[button] = BTN_FSM_RELEASE_CHECK;     // Переход к следующему состоянию
      return true;                                   // Позитивный выход
    } else {                                         // Если счётчик не достиг предела...
      fsm_state[button] = BTN_FSM_PRESS_CHECK;       // Переход к начальному состоянию
      return false;                                  // Негативный выход
    }
  }

  // Состояние «Ожидание отпускания»
  if (fsm_state[button] == BTN_FSM_RELEASE_CHECK) {

    // Проверка состояния кнопки
    if (((bool)(*port & mask) ^ pull) == false) {    // Если кнопка отпущена...
      fsm_state[button] = BTN_FSM_RELEASE_DEBOUNCE;  // Переход к следующему состоянию
    } else {                                         // Если кнопка не отпущена...
      return false;                                  // Негативный выход
    }
  }

  // Состояние «Защита от дребезга при отпускании»
  if (fsm_state[button] == BTN_FSM_RELEASE_DEBOUNCE) {

    // Сброс счётчика
    counter = 0;

    // Цикл счёта
    for (int i = 0; i < BTN_CNT_LIM; i++) {
      counter += !((bool)(*port & mask) ^ pull);
    }

    // Проверка состояния накопителя
    if (counter == BTN_CNT_LIM) {                    // Если счётчик достиг предела...
      fsm_state[button] = BTN_FSM_PRESS_CHECK;       // Переход к начальному состоянию
    } else {                                         // Если счётчик не достиг предела...
      fsm_state[button] = BTN_FSM_RELEASE_CHECK;     // Переход к предыдущему состоянию
    }
    return false;                                    // Негативный выход
  }

  // Во всех неопределенных случаях
  // производится негативный выход
  return false;
}
