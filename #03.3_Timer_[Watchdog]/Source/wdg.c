/******************************************************************************
 * @File: wdg.c
 * @Author: Milandr, L.
 * @Project: #03.3_Timer_[Watchdog]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 08.04.2020
 * @Purpose: Конфигурация сторожевого таймера
 ******************************************************************************/

// Подключение заголовка
#include "wdg.h"

// Инициализация генератора LSI
void LSI_Init(void)
{
  // Включение генератора
  MDR_BKP->REG_0F |= BKP_REG_0F_LSI_ON;

  // Ожидание входа в рабочий режим
  while ((MDR_BKP->REG_0F & BKP_REG_0F_LSI_RDY) == false);

  // Подстройка частоты
  MDR_BKP->REG_0F &= ~BKP_REG_0F_LSI_TRIM_Msk;         // Сброс битового поля
  MDR_BKP->REG_0F |= (31 << BKP_REG_0F_LSI_TRIM_Pos);  // Установка значения
}

// Инициализация сторожевого таймера
void IWDG_Init(void)
{
  // Инициализация генератора LSI
  LSI_Init();

  // Включение тактирования сторожевого таймера
  MDR_RST_CLK->PER_CLOCK |= RST_CLK_PCLK_IWDT_Msk;

  // Верификация периода перезагрузки:
  // если период вне допустимого диапазона...
  if (IWDG_RLD_TIME < 1
   || IWDG_RLD_TIME > IWDG_RLR_MAX * (4 << IWDG_PR_MAX) * 1000 / LSI_CLK) {

    // Отображение диагностического сообщения
    LCD_PrintString("    WDG DISABLED!", 5);

    // Выход из функции
    return;
  }

  // Инициализация переменных
  uint32_t rlr = 0;  // Цифровой период
  uint8_t  pr  = 0;  // Делитель частоты

  // Исходное вычисление цифрового периода
  rlr = IWDG_RLR_MAX * LSI_CLK / (4 << pr) / 1000;

  // Цикл вычисления допустимых значений периода и делителя;
  // выполняется, пока: период вне допуска & делитель в допуске
  while (rlr >= IWDG_RLR_MAX
      && pr  <= IWDG_PR_MAX) {

    // Инкремент делителя
    pr++;

    // Повторное вычисление периода
    rlr = IWDG_RLD_TIME * LSI_CLK / (4 << pr) / 1000;
  }

  // Разрешение модификации регистров PR и RLR
  MDR_IWDG->KR = IWDG_KR_UNLOCK;

  // Установка делителя (с проверкой флага обновления - PVU)
  while ((MDR_IWDG->SR & IWDG_SR_PVU) != false);
  MDR_IWDG->PR = pr;

  // Установка периода (с проверкой флага обновления - RVU)
  while ((MDR_IWDG->SR & IWDG_SR_RVU) != false);
  MDR_IWDG->RLR = rlr;

  // Перезагрузка таймера
  // (требуется для корректной установки параметров)
  MDR_IWDG->KR = IWDG_KR_RELOAD;

  // Запуск сторожевого таймера
  MDR_IWDG->KR = IWDG_KR_LAUNCH;
}
