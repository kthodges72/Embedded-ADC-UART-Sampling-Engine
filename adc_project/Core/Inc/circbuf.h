#ifndef CIRCBUF_H
#define CIRCBUF_H


#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#define CIRC_BUF_SIZE 256


typedef struct {

	uint16_t head;
	uint16_t tail;
	uint16_t size;
	uint8_t buffer[CIRC_BUF_SIZE];

} CircBuf;

extern CircBuf txbuf;


/**
  * @brief  Initialize circbuf module
  * @param  *circbuf Pointer to the CircBuf instance
  * @retval Void
**/
void circbuf_init(CircBuf *circbuf);

/**
 *  @brief  Set urn pointer to start of contiguous chunk (buffer values until end of circular buffer),
 *          as well as length of contiguous chunk
 *  @param  *circbuf Pointer to CircBuf instance
  * @param  **ptr Pointer to pointer to start of contiguous chunk
  * @param  *len Pointer to length of contiguous chunk
  * @retval Void
**/
void circbuf_peek_contiguous(CircBuf *circbuf, uint8_t **ptr, uint16_t *len);

/**
  * @brief  Advance tail the length of the chunk that was just sent over DMA
  * @param  *circbuf Pointer to the CircBuf instance
  * @param  len Length of chunk that was just sent over DMA
  * @retval Void
**/
void circbuf_advance(CircBuf *circbuf, uint16_t len);

/**
  * @brief  Return int of count of values currently in buffer
  * @param  *circbuf Pointer to the CircBuf instance
  * @retval 16 bit int of count of values currently in buffer
**/
uint16_t circbuf_count(CircBuf *circbuf);

/**
  * @brief  Write a single byte to buffer, advance head
  * @note   If buffer full, it overwrites the oldest byte
  * @param  *circbuf Pointer to the CircBuf instance
  * @param  byte Byte of data to by written to buffer (char, ADC value)
  * @retval Void
**/
void circbuf_write_byte(CircBuf *circbuf, uint8_t byte);

/**
  * @brief  Return true if buffer is empty
  * @param  *circbuf Pointer to the CircBuf instance
  * @retval - true if empty
  * 		- false if not empty
**/
bool circbuf_is_empty(CircBuf *circbuf);

/**
  * @brief  Return true if buffer is full
  * @param  *circbuf Pointer to the CircBuf instance
  * @retval - true if full
  * 		- false if not full
**/
bool circbuf_is_full(CircBuf *circbuf);


#endif

