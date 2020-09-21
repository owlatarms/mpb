/******************************************************************************
 * @File: lib_lcd.c
 * @Author: Milandr, L.
 * @Project: Library
 * @Version: 1.0.2
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 16.03.2020
 * @Purpose: Библиотека для работы с дисплеем MT-12864B (драйвер NT75451)
 ******************************************************************************/

// Подключение заголовка
#include "lib_lcd.h"

// Инициализация ЖК-дисплея
void LCD_Init(void)
{
  // Включение тактирования портов E и F
  MDR_RST_CLK->PER_CLOCK |= RST_CLK_PCLK_PORTE_Msk
                          | RST_CLK_PCLK_PORTF_Msk;

  // Конфигурация служебных линий PE1 (A0), PE2 (LED)
  MDR_PORTE->OE     |=  0x00000006;  // Направление (вывод)
  MDR_PORTE->FUNC   &= ~0x0000003C;  // Функция (ввод-вывод)
  MDR_PORTE->ANALOG |=  0x00000006;  // Режим (цифровой)
  MDR_PORTE->PULL   &= ~0x00060006;  // Подтяжка (отключена)
  MDR_PORTE->PD     &= ~0x00060006;  // Управление (драйвер)
  MDR_PORTE->PWR    |=  0x0000003C;  // Крутизна (высокая)
  MDR_PORTE->GFEN   &= ~0x00000006;  // Фильтр (не используется)

  // Конфигурация линий PF0 (MOSI), PF1 (SCLK), PF2 (CS) и PF3 (MISO)
  MDR_PORTF->OE     &= ~0x0000000F;  // Направление (не используется)
  MDR_PORTF->FUNC   &= ~0x000000FF;  // Сброс битов регистра
  MDR_PORTF->FUNC   |=  0x000000AA;  // Функция (альтернативная)
  MDR_PORTF->ANALOG |=  0x0000000F;  // Режим (цифровой)
  MDR_PORTF->PULL   &= ~0x000F000F;  // Подтяжка (отключена)
  MDR_PORTF->PD     &= ~0x000F000F;  // Управление (не используется)
  MDR_PORTF->PWR    |=  0x000000FF;  // Крутизна (высокая)
  MDR_PORTF->GFEN   &= ~0x0000000F;  // Фильтр (не используются)

  // Включение тактирования SPI1
  MDR_RST_CLK->PER_CLOCK |= RST_CLK_PCLK_SSP1_Msk;
  MDR_RST_CLK->SSP_CLOCK |= RST_CLK_SSP_CLOCK_SSP1_CLK_EN;

  // Деинициализация SPI1
  MDR_SSP1->CR0   = 0;
  MDR_SSP1->CR1   = 0;
  MDR_SSP1->CPSR  = 0;
  MDR_SSP1->IMSC  = 0;
  MDR_SSP1->DMACR = 0;

  // Конфигурация SPI1
  MDR_SSP1->CR0 = (7 << SSP_CR0_DSS_Pos)   // Длина пакета данных (8 бит)
                | (0 << SSP_CR0_FRF_Pos)   // Формат пакета данных (SPI)
                | (1 << SSP_CR0_SPO_Pos)   // Полярность тактовых импульсов (CPOL = 1)
                | (1 << SSP_CR0_SPH_Pos)   // Фронт выборки данных (передний, CPHA = 1)
                | (0 << SSP_CR0_SCR_Pos);  // Фактор скорости передачи данных (не используется)

  MDR_SSP1->CR1 = (0 << SSP_CR1_LBM_Pos)   // Режим самотестирования (отключён)
                | (1 << SSP_CR1_SSE_Pos)   // Работа приёмопередатчика (включён)
                | (0 << SSP_CR1_MS_Pos)    // Роль в сети (ведущий)
                | (0 << SSP_CR1_SOD_Pos);  // Отключение линии передачи (не используется)

  // Установка делителя тактовой частоты
  MDR_SSP1->CPSR = SystemCoreClock / LCD_SPI_SPEED;

  // Запись конфигурационной таблицы
  for (int i = 0; i < sizeof(lcd_cfg_table); i++) {
    LCD_WriteCmd(lcd_cfg_table[i]);
  }

  // Очистка дисплея
  LCD_ClearScreen();

  // Отображение логотипа компании «Миландр»
  LCD_PrintLogo();

  // Включение подсветки дисплея
  LCD_LED_ON;
}

// Запись командного слова
void LCD_WriteCmd(uint8_t cmd)
{
  // Перевод дисплея в режим команд
  LCD_MODE_CMD;

  // Запись командного слова
  MDR_SSP1->DR = cmd;

  // Ожидание завершения обмена
  while ((MDR_SSP1->SR & SSP_SR_BSY) != false);
}

// Запись слова данных
void LCD_WriteData(uint8_t data)
{
  // Перевод дисплея в режим данных
  LCD_MODE_DATA;

  // Запись слова данных
  MDR_SSP1->DR = data;

  // Ожидание завершения обмена
  while ((MDR_SSP1->SR & SSP_SR_BSY) != false);
}

// Выбор страницы
void LCD_SetPage(uint8_t page)
{
  // Запись командного слова со страницей
  LCD_WriteCmd(0xB0 | page);
}

// Выбор столбца
void LCD_SetColumn(uint8_t column)
{
  // Запись командного слова со столбцом (4 LSB)
  LCD_WriteCmd(column & 0x0F);

  // Запись командного слова со столбцом (4 MSB)
  LCD_WriteCmd((column >> 4) | 0x10);
}

// Установка указателя (x = 0...20, y = 0...7)
void LCD_SetPointer(uint8_t x, uint8_t y)
{
  // Выбор требуемой страницы
  LCD_SetPage(y % 8);
  
  // Выбор требуемого столбца
  LCD_SetColumn((x % 21) * 6 + 1);
}

// Инверсия цвета
void LCD_Inverse(bool state)
{
  if (state != false) {
    LCD_WriteCmd(0xA7);  // Активация инверсии
  } else {
    LCD_WriteCmd(0xA6);  // Деактивация инверсии
  }
}

// Очистка строки
void LCD_ClearString(uint8_t line)
{
  // Выбор требуемой строки (страницы)
  LCD_SetPointer(0, line);

  // Цикл очистки столбцов
  for (uint8_t i = 0; i < 128; i++) {
    LCD_WriteData(0x00);
  }
}

// Полная очистка
void LCD_ClearScreen(void)
{
  // Цикл очистки всех страниц
  for (uint8_t i = 0; i < 8; i++) {

    // Выбор страницы
    LCD_SetPage(i);

    // Выбор исходного столбца
    LCD_SetColumn(0);

    // Цикл очистки столбцов
    for (uint8_t j = 0; j < 128; j++) {
      LCD_WriteData(0x00);
    }
  }
}

// Отображение символа
void LCD_PrintSymbol(uint32_t symbol, uint8_t x, uint8_t y)
{
  uint16_t index = 0;  // Индекс символа в таблице шрифта

  // Если используется кодировка UTF-8
  #ifdef USE_UTF8

  uint8_t  sym_len = 0;  // Длина закодированного символа (в байтах)
  uint16_t unicode = 0;  // Кодировка символа в формате Unicode

  // Перекодирование символа из UTF-8 в Unicode
  // Обработка младших байтов
  for (sym_len = 0; ((symbol >> (sym_len * 8)) & 0xC0) == 0x80; sym_len++) {
    unicode |= (symbol >> (sym_len * 8) & 0x3F) << (sym_len * 6);
  }

  // Обработка 1-байтовых символов
  if (sym_len == 0) {
    unicode = symbol & 0xFF;
  }

  // Обработка старшего байта
  else {
    unicode |= ((symbol >> (sym_len * 8)) & (0xFF >> (sym_len + 2))) << (sym_len * 6);
  }

  // Определение индекса символа в таблице кодировок
  // Латиница, цифры и прочее
  if (unicode >= FONT_LAT_MIN_CODE && unicode <= FONT_LAT_MAX_CODE) {
    index = (unicode - FONT_LAT_MIN_CODE) * 6;
  }

  // Кириллица
  else if (unicode >= FONT_CYR_MIN_CODE && unicode < FONT_CYR_MAX_CODE) {
    index = (unicode - FONT_LAT_MIN_CODE - FONT_CYR_DIF_CODE) * 6;
  }

  // Исключения
  else if (unicode == 0x0401) {index = (0xA8 - 0x20) * 6;}  // Ё
  else if (unicode == 0x0451) {index = (0xB8 - 0x20) * 6;}  // ё
  else if (unicode == 0x2116) {index = (0xB9 - 0x20) * 6;}  // №

  // Символы вне таблицы заменяются пробелом
  else {
    index = 0;
  }

  // Если используется кодировка CP1251
  #else

  // Определение индекса символа в таблице кодировок
  if (symbol >= FONT_MIN_CODE && symbol <= FONT_MAX_CODE) {
    index = (symbol - FONT_MIN_CODE) * 6;
  }

  // Символы вне таблицы заменяются пробелом
  else {
    index = 0;
  }

  #endif

  // Установка указателя
  LCD_SetPointer(x, y);

  // Цикл отображения символа
  for (int i = 0; i < 6; i++) {
    LCD_WriteData(font[index + i]);
  }
}

// Отображение строки
void LCD_PrintString(const char *str, uint8_t y, ...)
{
  uint8_t str_len = 0;    // Длина строки
  uint8_t sym_pos = 0;    // Позиция символа по оси абсцисс
  va_list arg_ptr = {0};  // Указатель на список аргументов

  // Инициализация указателя
  va_start(arg_ptr, y);

  // Форматирование строки
  char strf[LCD_SCREEN_WIDTH * 16];
  vsnprintf(strf, LCD_SCREEN_WIDTH * 16, str, arg_ptr);

  // Восстановление стека
  va_end(arg_ptr);

  // Вычисление длины строки (в байтах)
  str_len = strlen(strf) + 1;

  // Если используется кодировка в формате UTF-8
  #ifdef USE_UTF8

  uint8_t  sym_len = 0;  // Длина закодированного символа (в байтах)
  uint32_t utf_sym = 0;  // Кодировка символа в формате UTF-8

  // Цикл формирования строки
  for (int i = 0; i < str_len && sym_pos < LCD_SCREEN_WIDTH; utf_sym = 0) {

    // Остановка цикла, если достигнут конец строки
    if (strf[i] == '\0')
      break;

    // Определение длины закодированного символа
    while (strf[i] & (0x80 >> sym_len)) {
      sym_len++;
    }

    // Формирование символа в формате UTF-8
    // Однобайтовая кодировка
    if (sym_len == 0) {
      utf_sym = strf[i];
      i++;
    }

    // Многобайтовая кодировка
    else {
      for (; sym_len > 0; i++, sym_len--) {
        utf_sym |= (strf[i] << ((sym_len - 1) * 8));
      }
    }

    // Обработка символа перевода строки
    if (utf_sym == '\n') {
      y++;
      sym_pos = 0;
    }

    // Отображение символа
    else {
      LCD_PrintSymbol(utf_sym, sym_pos, y);
      sym_pos++;
    }
  }

  // Если используется кодировка в формате CP1251
  #else

  // Цикл формирования строки
  for (uint8_t i = 0; i < str_len; i++, sym_pos++) {

    // Остановка цикла, если достигнут конец строки
    if (strf[i] == '\0') {
      break;
    }

    // Отображение символа
    LCD_PrintSymbol(strf[i], sym_pos, y);
  }

  #endif

  // Заполнение оставшейся части строки пробелами
  for (; sym_pos < LCD_SCREEN_WIDTH; sym_pos++) {
    LCD_PrintSymbol(' ', sym_pos, y);
  }
}

// Отображение логотипа
void LCD_PrintLogo(void)
{
  // Переменная для циклов
  uint8_t i;

  // Установка указателя на нулевую строку
  LCD_SetPointer(0, 0);

  // Перевод дисплея в режим данных
  LCD_MODE_DATA;

  // Цикл отображения верхней половины логотипа
  for (i = 0; i < 126; i++) {
    LCD_WriteData(logo[i]);
  }

  // Установка указателя на первую строку
  LCD_SetPointer(0, 1);

  // Цикл отображения нижней половины логотипа
  for (; i < 252; i++) {
    LCD_WriteData(logo[i]);
  }
}
