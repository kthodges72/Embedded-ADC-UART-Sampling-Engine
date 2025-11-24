#include "uart.h"
#include "stm32f4xx_ll_usart.h"
#include "stm32f4xx_ll_dma.h"
#include "circbuf.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

UART_Handle_t uart;

/**
  * @brief  Initialize uart_init module
  * @param  *uart Pointer to the UART_Handle_t instance
  * @param  *circ_buf Pointer to CircBuf instance
  * @retval Void
**/
void uart_init(UART_Handle_t *uart, CircBuf *circ_buf) {

	// initialize software state
	uart->Instance = USART2;
	uart->DMA_Stream = LL_DMA_STREAM_6;
	uart->circ_buffer = circ_buf;
	uart->tx_buffer = NULL;
	uart->tx_length = 0;
	uart->tx_busy = false;

	// ensure DMA stream 6 is disabled
	LL_DMA_DisableStream(DMA1, uart->DMA_Stream);

	// clear all DMA flags
	LL_DMA_ClearFlag_TC6(DMA1);
	LL_DMA_ClearFlag_TE6(DMA1);
	LL_DMA_ClearFlag_FE6(DMA1);
	LL_DMA_ClearFlag_DME6(DMA1);
	LL_DMA_ClearFlag_HT6(DMA1);

	// set DMA peripheral address
	LL_DMA_SetPeriphAddress(DMA1, uart->DMA_Stream, (uint32_t)&USART2->DR);

	// enable USART DMA TX request
	LL_USART_EnableDMAReq_TX(uart->Instance);

	// enable DMA transfer complete interrupt
	LL_DMA_EnableIT_TC(DMA1, uart->DMA_Stream);

}

/**
  * @brief  Send largest contiguous chunk of data from circular buffer over DMA
  * @param  *uart Pointer to the UART_Handle_t instance
  * @retval Void
**/
void uart_send_dma(UART_Handle_t *uart) {

	// check if tx busy; return
	if (uart->tx_busy) {
		return;
	}

	// store buffer ptr and len values
	uint8_t *chunk_ptr;
	uint16_t chunk_len;
	circbuf_peek_contiguous(uart->circ_buffer, &chunk_ptr, &chunk_len);

	// set tx_buffer and tx_length fields
	uart->tx_buffer = chunk_ptr;
	uart->tx_length = chunk_len;
	if (uart->tx_length == 0) {
		return;
	}

	// set tx state to busy
	uart->tx_busy = true;

	// wait until DMA stream is fully idle
	LL_DMA_DisableStream(DMA1, uart->DMA_Stream);
	while (LL_DMA_IsEnabledStream(DMA1, uart->DMA_Stream));

	// set DMA memory address and transfer length
	LL_DMA_SetMemoryAddress(DMA1, uart->DMA_Stream, (uint32_t)uart->tx_buffer);
	LL_DMA_SetDataLength(DMA1, uart->DMA_Stream, uart->tx_length);

	// enable stream (start transfer)
	LL_DMA_EnableStream(DMA1, uart->DMA_Stream);

	return;

}

/**
  * @brief  reset flags after DMA tranfer complete, recur if more data in buffer
  * @param  *uart Pointer to the UART_Handle_t instance
  * @retval Void
**/
void uart_handle_dma_irq(UART_Handle_t *uart) {

	// if transfer complete flag enabled, clear flag, set tx_busy to false
	if (LL_DMA_IsActiveFlag_TC6(DMA1)) {
		LL_DMA_ClearFlag_TC6(DMA1);
		uart->tx_busy = false;

		// advance circbuf by tx_length
		circbuf_advance(uart->circ_buffer, uart->tx_length);

		// if more data, send again
		if (circbuf_count(uart->circ_buffer) > 0) {
			uart_send_dma(uart);
		}
	}

	// clear all other potential flags
	if (LL_DMA_IsActiveFlag_TE6(DMA1)) {
		LL_DMA_ClearFlag_TE6(DMA1);
	}
	if (LL_DMA_IsActiveFlag_DME6(DMA1)) {
		LL_DMA_ClearFlag_DME6(DMA1);
	}
	if (LL_DMA_IsActiveFlag_FE6(DMA1)) {
		LL_DMA_ClearFlag_FE6(DMA1);
	}

	return;
}

/**
  * @brief  Print sring to console using polling
  * @note   Used for debugging
  * @param  *uart Pointer to the UART_Handle_t instance
  * @param  *str  Pointer to first char in string
  * @retval Void
**/
void uart_printf(UART_Handle_t *uart, char *str) {

	while (uart->tx_busy) {
	}

    while (*str != '\0') {

        while (!LL_USART_IsActiveFlag_TXE(uart->Instance)) {
        }

        LL_USART_TransmitData8(uart->Instance, *str);
        str++;
    }
    while (!LL_USART_IsActiveFlag_TC(uart->Instance)) {
    }

}



/**
  * @brief  Print string to console using circular buffer and DMA
  * @note   This function uses the 'uart_send_dma' function
  * @param  *uart Pointer to the UART_Handle_t instance
  * @param  *str  Pointer to first char in string
  * @retval Void
**/
void uart_DMA_printf(UART_Handle_t *uart, char *str)
{
    // write all characters to circular buffer
    while (*str != '\0') {
        circbuf_write_byte(uart->circ_buffer, *str);
        str++;
    }

    // send DMA if not already active
    while (uart->tx_busy) {
    }
        uart_send_dma(uart);
}

