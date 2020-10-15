/******************************************************************************
 * @File: led.c
 * @Author: Milandr, L.
 * @Project: #02.3_GPIO_[LCD&Menu]
 * @Version: 1.1.0
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 15.10.2020
 * @Purpose: Конфигурация портов для работы светодиодов
 ******************************************************************************/

// Подключение заголовка
#include "led.h"

// Буфер данных элементов меню
char m_data[LED_NUM][2] = {NULL};

// Создание элементов меню
// Основная страница
MENU_ADD(m_p0i0, m_p0i1, m_null, m_p1i0, NULL,        0, 0, NULL,      "    LED 0:6");
MENU_ADD(m_p0i1, m_p0i2, m_p0i0, m_p3i0, NULL,        0, 1, NULL,      "    LED RGB");
MENU_ADD(m_p0i2, m_null, m_p0i1, m_null, LED_Reset,   0, 2, NULL,      "    Reset");

// Страница LED 0...3
MENU_ADD(m_p1i0, m_p1i1, m_null, m_null, LED_Handler, 1, 0, m_data[0], "    LED 0: %s");
MENU_ADD(m_p1i1, m_p1i2, m_p1i0, m_null, LED_Handler, 1, 1, m_data[1], "    LED 1: %s");
MENU_ADD(m_p1i2, m_p1i3, m_p1i1, m_null, LED_Handler, 1, 2, m_data[2], "    LED 2: %s");
MENU_ADD(m_p1i3, m_p2i0, m_p1i2, m_null, LED_Handler, 1, 3, m_data[3], "    LED 3: %s");

// Страница LED 4...6
MENU_ADD(m_p2i0, m_p2i1, m_p1i3, m_null, LED_Handler, 2, 0, m_data[4], "    LED 4: %s");
MENU_ADD(m_p2i1, m_p2i2, m_p2i0, m_null, LED_Handler, 2, 1, m_data[5], "    LED 5: %s");
MENU_ADD(m_p2i2, m_p2i3, m_p2i1, m_null, LED_Handler, 2, 2, m_data[6], "    LED 6: %s");
MENU_ADD(m_p2i3, m_null, m_p2i2, m_p0i0, NULL,        2, 3, NULL,      "    Return");

// Страница LED RGB
MENU_ADD(m_p3i0, m_p3i1, m_null, m_null, LED_Handler, 3, 0, m_data[7], "    LED R: %s");
MENU_ADD(m_p3i1, m_p3i2, m_p3i0, m_null, LED_Handler, 3, 1, m_data[8], "    LED G: %s");
MENU_ADD(m_p3i2, m_p3i3, m_p3i1, m_null, LED_Handler, 3, 2, m_data[9], "    LED B: %s");
MENU_ADD(m_p3i3, m_null, m_p3i2, m_p0i1, NULL,        3, 3, NULL,      "    Return");

// Инициализация системы для работы светодиодов
void LED_Init(void)
{
  // Включение тактирования портов A и B
  MDR_RST_CLK->PER_CLOCK |= RST_CLK_PCLK_PORTA_Msk
                          | RST_CLK_PCLK_PORTB_Msk;

  // Конфигурация линий PA0...PA7
  MDR_PORTA->OE     |=  0x000000FF;  // Направление (вывод)
  MDR_PORTA->FUNC   &= ~0x0000FFFF;  // Функция (ввод-вывод)
  MDR_PORTA->ANALOG |=  0x000000FF;  // Режим (цифровой)
  MDR_PORTA->PULL   &= ~0x00FF00FF;  // Подтяжка (отключена)
  MDR_PORTA->PD     &= ~0x00FF00FF;  // Управление (драйвер)
  MDR_PORTA->PWR    |=  0x0000FFFF;  // Крутизна (высокая)
  MDR_PORTA->GFEN   &= ~0x000000FF;  // Фильтр (не используется)

  // Конфигурация линий PB8...PB10
  MDR_PORTB->OE     |=  0x00000700;  // Направление (вывод)
  MDR_PORTB->FUNC   &= ~0x003F0000;  // Функция (ввод-вывод)
  MDR_PORTB->ANALOG |=  0x00000700;  // Режим (цифровой)
  MDR_PORTB->PULL   &= ~0x07000700;  // Подтяжка (отключена)
  MDR_PORTB->PD     &= ~0x07000700;  // Управление (драйвер)
  MDR_PORTB->PWR    |=  0x003F0000;  // Крутизна (высокая)
  MDR_PORTB->GFEN   &= ~0x00000700;  // Фильтр (не используется)

  // Исходное отключение светодиодов
  LED_Reset();

  // Инициализация меню
  MENU_Init(&m_p0i0);
}

// Сброс состояния светодиодов
void LED_Reset(void)
{
  // Отключение всех светодиодов
  MDR_PORTA->RXTX &= ~0x000000FF;
  MDR_PORTB->RXTX = (MDR_PORTB->RXTX & ~PORT_JTAG_Msk) & 0x00000700;

  // Цикл сброса данных элементов
  for (int i = 0; i < LED_NUM; i++) {
    strcpy(m_data[i], "0");
  }
}

// Обновление состояния светодиодов
void LED_Handler(void)
{
  // Параметры линии
  uint32_t *port = 0;  // Адрес порта
  uint16_t  mask = 0;  // Маска

  // Определение параметров
       if (m_curr == &m_p1i0) {port = (uint32_t *)&MDR_PORTA->RXTX, mask = 0x0001;}
  else if (m_curr == &m_p1i1) {port = (uint32_t *)&MDR_PORTA->RXTX, mask = 0x0002;}
  else if (m_curr == &m_p1i2) {port = (uint32_t *)&MDR_PORTA->RXTX, mask = 0x0004;}
  else if (m_curr == &m_p1i3) {port = (uint32_t *)&MDR_PORTA->RXTX, mask = 0x0008;}
  else if (m_curr == &m_p2i0) {port = (uint32_t *)&MDR_PORTA->RXTX, mask = 0x0010;}
  else if (m_curr == &m_p2i1) {port = (uint32_t *)&MDR_PORTA->RXTX, mask = 0x0020;}
  else if (m_curr == &m_p2i2) {port = (uint32_t *)&MDR_PORTA->RXTX, mask = 0x0040;}
  else if (m_curr == &m_p3i0) {port = (uint32_t *)&MDR_PORTB->RXTX, mask = 0x0200;}
  else if (m_curr == &m_p3i1) {port = (uint32_t *)&MDR_PORTB->RXTX, mask = 0x0100;}
  else if (m_curr == &m_p3i2) {port = (uint32_t *)&MDR_PORTB->RXTX, mask = 0x0400;}

  // Инверсия состояния светодиода
  if (port == (uint32_t *)&MDR_PORTB->RXTX) {
    *port = (*port & ~PORT_JTAG_Msk) ^ mask;
  } else {
    *port ^= mask;
  }

  // Обновление данных
  snprintf(m_curr->data, 2, "%u", !!(*port & mask));

  // Отображение строки
  LCD_PrintString(m_curr->text, m_curr->pos + MENU_STR_POS, m_curr->data);

  // Отображение указателя
  MENU_PrintPtr(m_curr);
}
