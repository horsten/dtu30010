#ifndef _DMA_SERIAL_H_
#define _DMA_SERIAL_H_ 1

// Implementation of the UART functions using DMA for transmission, based on 30010_io.h

/* Includes ------------------------------------------------------------------*/
#include "stm32f30x_conf.h"
#include <stdio.h>

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define UART_BUFFER_LENGTH 256

/* Exported functions ------------------------------------------------------- */
/****************************/
/*** USB Serial Functions ***/
/****************************/
void uart_init(uint32_t baud);
void uart_put_char(uint8_t c);
uint8_t uart_get_char();
uint8_t uart_get_count();
void uart_clear();

#endif
