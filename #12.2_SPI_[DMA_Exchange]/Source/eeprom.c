/******************************************************************************
 * @File: eeprom.c
 * @Author: Milandr, L.
 * @Project: #12.2_SPI_[DMA_Exchange]
 * @Version: 1.0.0
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 06.08.2020
 * @Purpose: Функции работы с микросхемой памяти 25LCXXX (Microchip)
 * @Description:
 * Адресное пространство микросхемы 25LCXXX разбито на страницы. При чтении или 
 * записи данных используется автоинкремент адреса, но при переходе адреса на
 * другую физическую страницу должен быть начат новый цикл чтения/записи. Такой
 * механизм автоматизирован в функциях данного файла.
 ******************************************************************************/

// Подключение заголовка
#include "eeprom.h"

// Чтение регистра состояний
uint8_t EEPROM_ReadStatus(void)
{
  // Принятые данные
  uint8_t rx_data;

  // Активация линии CS
  SPI_CS_LOW;

  // Передача инструкции чтения регистра состояний
  SPI_RW(EEPROM_RDSR);

  // Приём данных
  rx_data = SPI_RW(0xAA);

  // Деактивация линии CS
  SPI_CS_HIGH;

  // Возврат принятых данных
  return rx_data;
}

// Разрешение записи данных
void EEPROM_WriteEnable(void)
{
  // Активация линии CS
  SPI_CS_LOW;

  // Передача инструкции разрешения записи данных
  SPI_RW(EEPROM_WREN);

  // Деактивация линии CS
  SPI_CS_HIGH;

  // Ожидание обновления состояния памяти
  while ((EEPROM_ReadStatus() & EEPROM_WEL) == false);
}

// Чтение данных из памяти
void EEPROM_ReadData(uint32_t address, uint8_t *data, size_t size)
{
  // Инициализация переменных
  uint32_t batch = 0;  // Длина серии чтения
  uint32_t index = 0;  // Индекс элемента в массиве

  // Цикл чтения данных
  // (состоит из серий)
  while (size != 0) {

    // Определение длины серии
    // Если прогнозируется переход между страницами...
    if (size > (address / EEPROM_PAGE_SIZE + 1) * EEPROM_PAGE_SIZE - address) {

      // То длина серии вычисляется как расстояние до конца страницы
      batch = (address / EEPROM_PAGE_SIZE + 1) * EEPROM_PAGE_SIZE - address;
    }

    // Иначе...
    else {

      // Длина серии равна длине цикла
      batch = size;
    }

    // Активация линии CS
    SPI_CS_LOW;

    // Передача инструкции чтения
    SPI_RW(EEPROM_READ);

    // Передача адреса
    SPI_RW(address >> 8);      // Старшие 8 бит
    SPI_RW(address & 0x00FF);  // Младшие 8 бит

    // Запуск цикла чтения данных
    while (SPI_Receive(data + index, batch) != SPI_TRX_INITIATED);

    // Ожидание завершения цикла
    while (SPI_Receive(data + index, batch) != SPI_TRX_COMPLETED);

    // Вычитание длины серии из общего кол-ва читаемых данных
    size -= batch;

    // Установка индекса и адреса
    index   += batch;
    address += batch;

    // Деактивация линии CS
    SPI_CS_HIGH;
  }
}

// Запись данных в память
void EEPROM_WriteData(uint32_t address, uint8_t *data, size_t size)
{
  // Инициализация переменных
  uint32_t batch = 0;  // Длина серии записи
  uint32_t index = 0;  // Индекс элемента в массиве

  // Цикл записи данных
  // (состоит из серий)
  while (size != 0) {

    // Определение длины серии
    // Если прогнозируется переход между страницами...
    if (size > (address / EEPROM_PAGE_SIZE + 1) * EEPROM_PAGE_SIZE - address) {

      // То длина серии вычисляется как расстояние до конца страницы
      batch = (address / EEPROM_PAGE_SIZE + 1) * EEPROM_PAGE_SIZE - address;
    }

    // Иначе...
    else {

      // Длина серии равна длине цикла
      batch = size;
    }

    // Разрешение записи данных
    EEPROM_WriteEnable();

    // Активация линии CS
    SPI_CS_LOW;

    // Передача инструкции записи
    SPI_RW(EEPROM_WRITE);

    // Передача адреса
    SPI_RW(address >> 8);      // Старшие 8 бит
    SPI_RW(address & 0x00FF);  // Младшие 8 бит

    // Запуск цикла записи данных
    while (SPI_Transmit(data + index, batch) != SPI_TRX_INITIATED);

    // Ожидание завершения цикла
    while (SPI_Transmit(data + index, batch) != SPI_TRX_COMPLETED);

    // Вычитание длины серии из общего кол-ва читаемых данных
    size -= batch;

    // Установка индекса и адреса
    index   += batch;
    address += batch;

    // Деактивация линии CS
    SPI_CS_HIGH;

    // Ожидание обновления состояния памяти
    while ((EEPROM_ReadStatus() & EEPROM_WIP) != false);
  }
}

// Стирание страницы памяти
void EEPROM_ErasePage(uint16_t page)
{
  // Адрес начала страницы
  uint32_t address = page * EEPROM_PAGE_SIZE;

  // Разрешение записи данных
  EEPROM_WriteEnable();

  // Активация линии CS
  SPI_CS_LOW;

  // Передача инструкции записи
  SPI_RW(EEPROM_WRITE);

    // Передача адреса
  SPI_RW(address >> 8);      // Старшие 8 бит
  SPI_RW(address & 0x00FF);  // Младшие 8 бит

  // Массив нулевых значений
  uint8_t data[64] = {0};

  // Запуск цикла стирания данных
  while (SPI_Transmit(data, EEPROM_PAGE_SIZE) != SPI_TRX_INITIATED);

  // Ожидание завершения цикла
  while (SPI_Transmit(data, EEPROM_PAGE_SIZE) != SPI_TRX_COMPLETED);

  // Деактивация линии CS
  SPI_CS_HIGH;

  // Ожидание обновления состояния памяти
  while ((EEPROM_ReadStatus() & EEPROM_WIP) != false);
}

// Полное стирание памяти
void EEPROM_EraseAll(void)
{
  // Цикл стирания данных
  for (uint32_t i = 0; i < EEPROM_DENSITY / EEPROM_PAGE_SIZE; i++) {
    EEPROM_ErasePage(i);
  }
}
