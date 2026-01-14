/**
 * uart.h
 * -------
 * UART transmission driver using DMA and a circular buffer.
 *
 * Provides non-blocking output over USART2 by
 * buffering characters (with circbuf module) and
 * transmitting the largest contiguous chunk via DMA.
 *
 * Designed for low CPU burden.
 **/

#ifndef UART_H
#define UART_H


#include "stm32f4xx.h"
#include "stm32f4xx_ll_dma.h"
#include "stm32f4xx_ll_usart.h"
#include "circbuf.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {

	USART_TypeDef* Instance; // which USART
	uint32_t DMA_Stream; // DMA stream index-pointer (for LL functions)
	CircBuf* circ_buffer; // pointer to circ buffer struct
	uint8_t* tx_buffer; // pointer to active buffer
	uint32_t tx_length; // number of bytes remaining
	volatile bool tx_busy; // is DMA busy?

} UART_Handle_t;

// global UART_Handle_t struct
extern UART_Handle_t uart;

/**
  * @brief  Initialize uart_init module
  * @param  *uart Pointer to the UART_Handle_t instance
  * @param  *circ_buf Pointer to CircBuf instance
  * @retval Void
**/
void uart_init(UART_Handle_t* uart, CircBuf* circ_buf);

/**
  * @brief  Send largest contiguous chunk of data from circular buffer over DMA
  * @param  *uart Pointer to the UART_Handle_t instance
  * @retval Void
**/
void uart_send_dma(UART_Handle_t* uart);

/**
  * @brief  reset flags after DMA tranfer complete, recur if more data in buffer
  * @param  *uart Pointer to the UART_Handle_t instance
  * @retval Void
**/
void uart_handle_dma_irq(UART_Handle_t* uart);

/**
  * @brief  Print sring to console using polling
  * @note   Used for debugging
  * @param  *uart Pointer to the UART_Handle_t instance
  * @param  *str  Pointer to first char in string
  * @retval Void
**/
void uart_DMA_printf(UART_Handle_t* uart, char* str);

/**
  * @brief  Print string to console using circular buffer and DMA
  * @note   This function uses the 'uart_send_dma' function
  * @param  *uart Pointer to the UART_Handle_t instance
  * @param  *str  Pointer to first char in string
  * @retval Void
**/
void uart_printf(UART_Handle_t* uart, char* str);

#endif
