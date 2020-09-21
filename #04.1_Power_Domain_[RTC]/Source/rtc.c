/******************************************************************************
 * @File: rtc.c
 * @Author: Milandr, L.
 * @Project: #04.1_Power_Domain_[RTC]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 28.04.2020
 * @Purpose: Конфигурация часов реального времени
 ******************************************************************************/

// Подключение заголовка
#include "rtc.h"

// Инициализация часов реального времени (RTC)
void RTC_Init(void)
{
  // Включение тактирования батарейного домена
  MDR_RST_CLK->PER_CLOCK |= RST_CLK_PCLK_BKP_Msk;

  // Включение тактирования RTC от HSE
  MDR_RST_CLK->RTC_CLOCK |= RST_CLK_RTC_CLOCK_HSE_RTC_EN;

  // Установка делителя частоты HSE:
  // 10 МГц / 16 = 625 КГц
  MDR_RST_CLK->RTC_CLOCK |= (11 << RST_CLK_RTC_CLOCK_HSE_SEL_Pos);

  // Сохранение состояния регистра REG_0F
  RTC_WAIT_FOR_UPDATE;
  uint32_t reg_0f = MDR_BKP->REG_0F;

  // Конфигурация RTC
  reg_0f |= (3 << BKP_REG_0F_RTC_SEL_Pos)  // Выбор источника тактирования (HSE)
          | (0 << BKP_REG_0F_CAL_Pos)      // Корректировка скорости хода
          | (1 << BKP_REG_0F_RTC_EN_Pos);  // Запуск RTC

  // Запись конфигурации в регистр REG_0F
  RTC_WAIT_FOR_UPDATE;
  MDR_BKP->REG_0F = reg_0f;

  // Установка делителя частоты RTC:
  // для получения частоты счёта 1 Гц
  RTC_WAIT_FOR_UPDATE;
  MDR_BKP->RTC_PRL = 625000;
}

// Отображение даты и времени
void RTC_PrintTime(time_t unix_time)
{
  // Структура компонентов времени
  struct tm *broken_time = NULL;

  // Разбиение Unix-времени на компоненты
  broken_time = localtime(&unix_time);

  // Отображение даты и времени
  // Время
  LCD_PrintString("      %02u:%02u:%02u", 3, broken_time->tm_hour,
                                             broken_time->tm_min,
                                             broken_time->tm_sec);
  // Дата
  LCD_PrintString("     %02u.%02u.%04u",  5, broken_time->tm_mday,
                                             broken_time->tm_mon  + 1,
                                             broken_time->tm_year + 1900);
  // Unix-время
  LCD_PrintString("     %010u",           7, unix_time);
}

// Установка даты и времени
void RTC_SetTime(void)
{
  // Структура компонентов времени
  struct tm broken_time = {0};

  // Формирование начального времени
  // Текущеее время
  // 28.04.2020, 12:53:41
  broken_time.tm_sec  = 0;            // Секунды (0...60)
  broken_time.tm_min  = 0;            // Минуты (0...59)
  broken_time.tm_hour = 12;           // Часы (0...23)
  broken_time.tm_mday = 28;           // День месяца (1...31)
  broken_time.tm_mon  = 4 - 1;        // Полных месяцев с начала года (0 - январь, 11 - декабрь)
  broken_time.tm_year = 2020 - 1900;  // Полных лет с 1900 года

  // Самая ранняя метка
  // 01.01.1970, 00:00:00
  /*
  broken_time.tm_sec  = 0;
  broken_time.tm_min  = 0;
  broken_time.tm_hour = 0;
  broken_time.tm_mday = 1;
  broken_time.tm_mon  = 1 - 1;
  broken_time.tm_year = 1970 - 1900;
  */

  // Имитация проблемы #1: за 10 секунд до возникновения
  // 19.01.2038, 03:13:57
  /*
  broken_time.tm_sec  = 57;
  broken_time.tm_min  = 13;
  broken_time.tm_hour = 3;
  broken_time.tm_mday = 19;
  broken_time.tm_mon  = 1 - 1;
  broken_time.tm_year = 2038 - 1900;
  */

  // Имитация проблемы #2: за 10 секунд до возникновения
  // 06.02.2106, 06:28:04
  /*
  broken_time.tm_sec  = 4;
  broken_time.tm_min  = 28;
  broken_time.tm_hour = 6;
  broken_time.tm_mday = 6;
  broken_time.tm_mon  = 2 - 1;
  broken_time.tm_year = 2106 - 1900;
  */

  // Синтез Unix-времени
  time_t unix_time = mktime(&broken_time);

  // Запись Unix-времени в RTC
  RTC_WAIT_FOR_UPDATE;
  MDR_BKP->RTC_CNT = unix_time;
}
