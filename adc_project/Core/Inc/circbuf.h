#ifndef CIRCBUF_H
#define CIRCBUF_H

#include <stdint.h>


typedef struct {
	uint32_t address;
	uint32_t size;
	uint8_t *head;
	uint8_t *tail;
	uint32_t len;
	double buffer[];
} CircBuf;

extern CircBuf txbuf;

void circbuf_init(CircBuf *circbuf);
void circbuf_peek_contiguous(CircBuf *circbuf, uint8_t **head, uint32_t *len);
void circbuf_advance(CircBuf *circbuf, uint32_t len);
int circbuf_count(CircBuf *circbuf);
void circbuf_write_byte(CircBuf *circbuf, uint8_t byte);


#endif

