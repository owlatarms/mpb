﻿/******************************************************************************
 * @File: main.c
 * @Author: Milandr, L.
 * @Project: #03.1_Timer_[General_Purpose]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 06.04.2020
 * @Purpose: Основной модуль
 * @Description:
 * В данном проекте выполняется инициализация двух аппаратных таймеров.
 * Первый таймер - циклический - поддерживает заданное время исполнения алгоритма.
 * Второй таймер - метрический - измеряет длительность сортировки некоторой выборки.
 * Также выполнена стабилизация тактовой частоты ядра и её разгон до 80 МГц.
 ******************************************************************************/

// Подключение заголовка
#include "main.h"

// Сортируемый массив
int sample[ARR_S] = {0};

// Основная функция
int main(void)
{
  // Общая инициализация
  CPU_Init();  // Система тактирования
  LCD_Init();  // ЖК-дисплей

  // Инициализация таймеров
  TIMER1_Init();  // Циклический таймер
  TIMER2_Init();  // Метрический таймер

  // Инициализация переменных
  uint32_t tmr_cnt = 0;  // Счётчик тактов метрического таймера
  bool valid = false;    // Валидатор сортировки

  // Запуск циклического таймера
  MDR_TIMER1->CNTRL |= TIMER_CNTRL_CNT_EN;

  // Основной цикл
  while (true) {

    // Заполнение выборки п/случайными элементами
    Randomize(sample, ARR_S, RANDOM_MAX);

    // Запуск метрического таймера
    MDR_TIMER2->CNTRL |= TIMER_CNTRL_CNT_EN;

    // Запуск сортировки
    BubbleSort(sample, ARR_S);

    // Остановка метрического таймера
    MDR_TIMER2->CNTRL &= ~TIMER_CNTRL_CNT_EN;

    // Вычисление затраченных тактов
    // с учётом количества перезагрузок
    tmr_cnt = MDR_TIMER2->CNT + tmr_rld_cnt * (MDR_TIMER2->ARR + 1);

    // Сброс счётчиков метрического таймера
    MDR_TIMER2->CNT = 0;
    tmr_rld_cnt = 0;

    // Валидация сортировки
    valid = Validate(sample, ARR_S);

    // Отображение результатов на дисплей
    LCD_PrintString("    T = %u т.", 3, tmr_cnt);  // Затраченные такты
    LCD_PrintString("    V = %u",    5, valid);    // Значение валидатора

    // Работа с циклическим таймером
    while (flg_tmr_eoc == false);  // Ожидание флага завершения счёта
    flg_tmr_eoc = false;           // Сброс флага
  }
}