/******************************************************************************
 * @File: lib_sort.c
 * @Author: Milandr, L.
 * @Project: Library
 * @Version: 1.1.0
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 31.03.2020
 * @Purpose: Функции сортировки
 ******************************************************************************/

// Подключение заголовка
#include "lib_sort.h"

// Сортировка пузырьком
void BubbleSort(int *sample, size_t size)
{
  // Инициализация переменных
  bool flg_swap = false;  // Флаг перестановки
  int  t = 0;             // Транзитная переменная

  // Цикл сортировки
  for (int i = 0; i < size - 1; i++) {

    // Сброс флага перестановки
    flg_swap = false;

    // Иттерация прохождения выборки
    for (int j = 0; j < size - 1 - i; j++) {

      // Сравнение элементов выборки
      if (sample[j] > sample[j + 1]) {

        // Перестановка
        t = sample[j];
        sample[j] = sample[j + 1];
        sample[j + 1] = t;

        // Установка флага перестановки
        flg_swap = true;
      }
    }

    // Если перестановок не зафиксировано -
    // сортировка завершается
    if (flg_swap == false) {
      break;
    }
  }
}

// Сортировка расчёской
void CombSort(int *sample, size_t size)
{
  // Инициализация переменных
  double factor = 1.247331;     // Фактор уменьшения
  int    step = size / factor;  // Шаг расчёски
  bool   flg_swap = false;      // Флаг перестановки
  int    t = 0;                 // Транзитная переменная

  // Фаза 1: расчёска
  while (step >= 1) {

    // Иттерация прохождения выборки
    for (int i = 0; i + step < size; i++) {

      // Сравнение элементов выборки
      if (sample[i] > sample[i + step]) {

        // Перестановка
        t = sample[i];
        sample[i] = sample[i + step];
        sample[i + step] = t;
      }
    }

    // Уменьшение шага расчёски
    step /= factor;
  }

  // Фаза 2: пузырёк
  for (int i = 0; i < size - 1; i++) {

    // Сброс флага перестановки
    flg_swap = false;

    // Иттерация прохождения выборки
    for (int j = 0; j < size - 1 - i; j++) {

      // Сравнение элементов выборки
      if (sample[j] > sample[j + 1]) {

        // Перестановка
        t = sample[j];
        sample[j] = sample[j + 1];
        sample[j + 1] = t;

        // Установка флага перестановки
        flg_swap = true;
      }
    }

    // Если перестановок не зафиксировано -
    // сортировка завершается
    if (flg_swap == false) {
      break;
    }
  }
}

// Сортировка вставками
void InsertSort(int *sample, size_t size)
{
  // Ключевой элемент сортировки
  int k = 0;

  // Цикл сортировки
  for (int i = 1; i < size; i++) {

    // Запись ключевого элемента
    k = sample[i];
    int j = i;

    // Сравнение ключевого элемента
    // с предшествующими элементами выборки
    for (; (j > 0) && (sample[j - 1]) > k; j--) {

      // Перестановка
      sample[j] = sample[j - 1];
    }

    // Фиксация позиции ключевого элемента
    sample[j] = k;
  }
}

// Сортировка Шелла
void ShellSort(int *sample, size_t size)
{
  // Инициализация переменных
  int step = size / 2;  // Шаг сравнения
  int t = 0;            // Транзитная переменная

  // Выбор шага
  while (step > 0) {

    // Иттерация прохождения выборки
    for (int i = step; i < size; i++) {

      // Сравнение элементов выборки
      for (int j = i - step; (j >= 0) && (sample[j] > sample[j + step]); j -= step) {

        // Перестановка
        t = sample[j];
        sample[j] = sample[j + step];
        sample[j + step] = t;
      }
    }

    // Уменьшение шага сравнения
    step /= 2;
  }
}

// Быстрая сортировка
void QuickSort(int *sample, size_t size)
{
  // Инициализация переменных
  int left = 0;          // Левая граница массива
  int right = size - 1;  // Правая граница массива
  int t = 0;             // Транзитная переменная

  // Опорный элемент (медиана)
  int pivot = sample[size / 2];

  // Цикл сдвига границ
  while (left <= right) {

    // Левая граница
    while (sample[left] < pivot) {
      left++;
    }

    // Правая граница
    while (sample[right] > pivot) {
      right--;
    }

    // Если границы не сомкнулись...
    if (left <= right) {

      // Перестановка
      t = sample[left];
      sample[left] = sample[right];
      sample[right] = t;

      // Сдвиг границ
      left++;
      right--;
    }
  }

  // Рекурсивная сортировка
  // Левая часть
  if (right > 0) {
    QuickSort(sample, right + 1);
  }

  // Правая часть
  if (left < size) {
    QuickSort(&sample[left], size - left);
  }
}

// Проверка сортировки
bool Validate(int *sample, size_t size)
{
  // Цикл проверки сортировки
  for (int i = 0; i < size - 1; i++) {

    // Если найден неотсортированный элемент,
    // то сортировна невалидна: негативный выход
    if (sample[i] > sample[i + 1]) {
      return false;
    }
  }

  // Если весь цикл пройден,
  // то сортировка валидна: позитивный выход
  return true;
}

// Генератор псевдослучайной выборки
void Randomize(int *sample, size_t size, uint32_t lim)
{
  // Цикл заполнения выборки псевдослучайными числами
  for (int i = 0; i < size; i++) {
    sample[i] = rand()%(lim + 1);
  }
}
