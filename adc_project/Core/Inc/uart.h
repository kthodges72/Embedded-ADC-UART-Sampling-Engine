#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx.h"
#include "stm32f4xx_ll_dma.h"
#include "stm32f4xx_ll_usart.h"
#include "circbuf.h"

typedef struct {
	USART_TypeDef *Instance; // which USART
	uint32_t DMA_Stream; // DMA stream index (for LL functions)
	CircBuf *circ_buffer; // pointer to circ buffer struct
	uint8_t *tx_buffer; // pointer to active buffer
	uint32_t tx_length; // number of bytes remaining
	volatile bool tx_busy; // is DMA busy?
	volatile bool data; // sending data?

} UART_Handle_t;

extern UART_Handle_t uart;

void uart_init(UART_Handle_t *uart, CircBuf *circ_buf);
void uart_send_dma(UART_Handle_t *uart);
void uart_handle_dma_irq(UART_Handle_t *uart);
void uart_DMA_printf(UART_Handle_t *uart, char *str);
void uart_printf(UART_Handle_t *uart, char *str);

#endif
