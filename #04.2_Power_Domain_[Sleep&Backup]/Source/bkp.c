/******************************************************************************
 * @File: bkp.c
 * @Author: Milandr, L.
 * @Project: #04.2_Power_Domain_[Sleep&Backup]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 29.04.2020
 * @Purpose: Конфигурация батарейного домена
 ******************************************************************************/

// Подключение заголовка
#include "bkp.h"

// Маскирование прерываний
void IRQ_Mask(uint8_t priority)
{
  // Псевдоним системного регистра BASEPRI
  register uint32_t BASEPRI __ASM("basepri");

  // Маскирование прерываний, преоритет которых ниже заданного (биты [7:5])
  // (обратите внимание: 0 - наивысший приоритет; 7 - самый низкий)
  BASEPRI = (priority << 5);
}

// Конфигурация режима гибернации
void CPU_SleepCfg(void)
{
  // Конфигурация режима гибернации
  SCB->SCR = (0 << SCB_SCR_SEVONPEND_Pos)     // Прерывание в качестве события (не используется)
           | (0 << SCB_SCR_SLEEPDEEP_Pos)     // Режим SLEEPDEEP (отключён)
           | (0 << SCB_SCR_SLEEPONEXIT_Pos);  // Возврат в режим гибернации (отключён)
}

// Переход в режим гибернации
void CPU_PutToSleep(void)
{
  // Маскирование низкоприоритетных прерываний
  IRQ_Mask(7);

  // Переход в режим гибернации
  __WFI();

  // Демаскирование прерываний
  // (выполняется по выходу из режима гибернации)
  IRQ_Mask(0);
}

// Инициализация детектора напряжения
void PVD_Init(void)
{
  // Включение тактирования детектора напряжения
  MDR_RST_CLK->PER_CLOCK |= RST_CLK_PCLK_POWER_Msk;

  // Конфигурация детектора напряжения
  MDR_POWER->PVDCS = (1 << POWER_PVDCS_PVDEN_Pos)  // Работа детектора (включён)
                   | (2 << POWER_PVDCS_PLS_Pos)    // Опорное напряжение (2.4 В)
                   | (1 << POWER_PVDCS_IEPVD_Pos)  // Прерывание от детектора (разрешено)
                   | (1 << POWER_PVDCS_INV_Pos);   // Область детектирования (нижняя)

  // Сброс запроса на обработку прерывания
  // (необходимо две итерации!)
  MDR_POWER->PVDCS &= ~POWER_PVDCS_PVD;
  MDR_POWER->PVDCS &= ~POWER_PVDCS_PVD;

  // Конфигурация контроллера NVIC
  NVIC_ClearPendingIRQ(POWER_IRQn);  // Сброс запросов прерываний
  NVIC_SetPriority(POWER_IRQn, 0);   // Приоритет прерываний
  NVIC_EnableIRQ(POWER_IRQn);        // Разрешение обработки прерываний
}

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
  reg_0f |= (3 << BKP_REG_0F_RTC_SEL_Pos)  // Источник тактирования (HSE)
          | (0 << BKP_REG_0F_CAL_Pos)      // Корректировка скорости хода
          | (1 << BKP_REG_0F_RTC_EN_Pos);  // Запуск RTC

  // Запись конфигурации в регистр REG_0F
  RTC_WAIT_FOR_UPDATE;
  MDR_BKP->REG_0F = reg_0f;

  // Установка делителя частоты RTC:
  // для получения частоты счёта 1 Гц
  RTC_WAIT_FOR_UPDATE;
  MDR_BKP->RTC_PRL = 625000;

  // Настройка запросов на обработку прерываний
  RTC_WAIT_FOR_UPDATE;
  MDR_BKP->RTC_CS |= BKP_RTC_CS_ALRF_IE;  // Прерывание при CNT = ALRM

  // Установка периода сигнализации RTC
  RTC_SET_ALARM(3);

  // Конфигурация контроллера NVIC
  NVIC_SetPriority(BACKUP_IRQn, 6);  // Приоритет прерываний
  NVIC_EnableIRQ(BACKUP_IRQn);       // Разрешение обработки прерываний
}

// Обработчик прерывания от детектора напряжения
void POWER_IRQHandler(void)
{
  // Сброс запроса на обработку прерывания
  // (необходимо две итерации!)
  MDR_POWER->PVDCS &= ~POWER_PVDCS_PVD;
  MDR_POWER->PVDCS &= ~POWER_PVDCS_PVD;

  // Запись счётчика RTC в регистр аварийного сохранения
  RTC_WAIT_FOR_UPDATE;
  MDR_BKP->REG_00 = MDR_BKP->RTC_CNT;
}

// Обработчик прерывания от батарейного домена
void BACKUP_IRQHandler(void)
{
  // Сброс запроса на обработку прерывания
  // (осуществляется записью единицы!)
  RTC_WAIT_FOR_UPDATE;
  MDR_BKP->RTC_CS |= BKP_RTC_CS_ALRF;

  // Установка периода сигнализации RTC
  RTC_SET_ALARM(3);
}
