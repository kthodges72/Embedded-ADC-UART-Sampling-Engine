/**
 * circbuf.c
 * ----------
 * Fixed-size circular buffer implementation.
 *
 * Provides overwrite-on-full logic
 *
 * Provides contiguous section 'length calculation'
 * for efficient DMA transfers.
 **/

#include "circbuf.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>


// initialize global CircBuf instance
CircBuf txbuf;

/**
  * @brief  Initialize circbuf module
  * @param  *circbuf Pointer to the CircBuf instance
  * @retval Void
**/
void circbuf_init(CircBuf* circbuf) {

	// set fields
    circbuf->head = 0;
    circbuf->tail = 0;
    circbuf->size = CIRC_BUF_SIZE;

}

/**
  * @brief  Return true if buffer is empty
  * @param  *circbuf Pointer to the CircBuf instance
  * @retval - true if empty
  * 		- false if not empty
**/
bool circbuf_is_empty(CircBuf* circbuf) {
	return circbuf->tail == circbuf->head;
}

/**
  * @brief  Return true if buffer is full
  * @param  *circbuf Pointer to the CircBuf instance
  * @retval - true if full
  * 		- false if not full
**/
bool circbuf_is_full(CircBuf* circbuf) {
	return ((circbuf->head + 1) % circbuf->size) == circbuf->tail;
}

/**
 *  @brief  Set pointer to start of contiguous chunk (buffer values until end of circular buffer),
 *          as well as length of contiguous chunk
 *  @param  Circbuf Pointer to CircBuf instance
  * @param  *ptr Pointer to pointer to start of contiguous chunk
  * @param  len Pointer to length of contiguous chunk
  * @retval Void
**/

void circbuf_peek_contiguous(CircBuf* circbuf, uint8_t** ptr, uint16_t* len) {

	// if buffer empty, return NULL pointer and length 0
	if (circbuf_is_empty(circbuf)) {
    	*len = 0;
    	*ptr = NULL;
    	return;
    }

	// set DMA pointer to point to tail
    *ptr = &circbuf->buffer[circbuf->tail];

    // set length
    if (circbuf->tail < circbuf->head) {
    	*len = circbuf->head - circbuf->tail; // if head is ahead of tail
    } else {
    	*len = circbuf->size - circbuf->tail; // if head wrapped around
    }
    return;
}

/**
  * @brief  Advance tail the length of the chunk that was just sent over DMA
  * @param  circbuf Pointer to the CircBuf instance
  * @param  len Length of chunk that was just sent over DMA
  * @retval Void
**/
void circbuf_advance(CircBuf* circbuf, uint16_t len) {

	// advance tail, taking wrap-around into account
	circbuf->tail = (circbuf->tail + len) % circbuf->size;

}

/**
  * @brief  Return int of count of values currently in buffer
  * @param  circbuf Pointer to the CircBuf instance
  * @retval uint16_t of count of values currently in buffer
**/
uint16_t circbuf_count(CircBuf* circbuf) {

	// return total count in buffer
	if (circbuf->head >= circbuf->tail)
	        return circbuf->head - circbuf->tail;
	    else
	        return circbuf->size - (circbuf->tail - circbuf->head);
}

/**
  * @brief  Write a single byte to buffer, advance head
  * @note   If buffer full, it overwrites the oldest byte
  * @param  *circbuf Pointer to the CircBuf instance
  * @param  byte Byte of data to by written to buffer (char, ADC value)
  * @retval Void
**/
void circbuf_write_byte(CircBuf* circbuf, uint8_t byte) {

    if (circbuf_is_full(circbuf)) {
    	circbuf->tail = (circbuf->tail + 1) % circbuf->size;
    }

	circbuf->buffer[circbuf->head] = byte;
    circbuf->head = (circbuf->head + 1) % circbuf->size;
}




