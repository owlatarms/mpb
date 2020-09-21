/******************************************************************************
 * @File: main.c
 * @Author: Milandr, L.
 * @Project: #15.1_CAN_[Simple_Exchange]
 * @Version: 1.0.1
 * @Compiler: ARM Compiler V5.06 (build 750)
 * @Microcontroller: К1986ВЕ92QI
 * @Device: Отладочная плата «МилКиТЭС»
 * @Date: 07.05.2020
 * @Purpose: Основной модуль
 * @Description:
 * В данном проекте реализована работа CAN в режиме самотестирования. Передача
 * кадра производится по нажатию кнопки. Принятый кадр отображается на дисплей.
 ******************************************************************************/

// Подключение заголовка
#include "main.h"

// Основная функция
int main(void)
{
  // Общая инициализация
  CPU_Init();   // Система тактирования
  TICK_Init();  // Системный таймер
  BTN_Init();   // Кнопки
  LCD_Init();   // ЖК-дисплей

  // Инициализация CAN
  CAN_Init();

  // Инициализация переменных
  frame_t rx_frame = {0};
  frame_t tx_frame = {0};

  // Формирование кадра
  tx_frame.id    = 123;
  tx_frame.datal = 256;
  tx_frame.datah = 0;

  // Отображение пустого кадра
  CAN_PrintFrame(rx_frame);

  // Основной цикл (пустой)
  while (true) {

    // Блок работы с передатчиком
    if (Wait(&mark[0], 100) == true) {

      // Обработка кнопки MID
      if (BTN_Handler(BTN_M) == true) {

        // Передача кадра
        CAN_SendFrame(tx_frame, TX_BUF);

        // Изменение данных кадра
        tx_frame.datal <<= 1;
      }
    }

    // Блок работы с приёмником
    if (Wait(&mark[1], 5) == true) {

      // Если флаг приёма кадра установлен...
      if (flg_frm_rcvd == true) {

        // Сброс флага приёма кадра
        flg_frm_rcvd = false;

        // Извлечение кадра из буфера
        rx_frame = CAN_ExtractFrame(RX_BUF);

        // Отображение содержания кадра
        CAN_PrintFrame(rx_frame);
      }
    }
  }
}
