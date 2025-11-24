#include "circbuf.h" // circbuf functions
#include <stdint.h>

CircBuf txbuf;

void circbuf_init(CircBuf *circbuf) {
    // implementation
}

void circbuf_peek_contiguous(CircBuf *circbuf, uint8_t **head, uint32_t *len) {
    // implementation
}

void circbuf_advance(CircBuf *circbuf, uint32_t len) {
    // implementation
}

int circbuf_count(CircBuf *circbuf) {
	// implementation
	return 0;
}

void circbuf_write_byte(CircBuf *circbuf, uint8_t byte) {
    // implementation
}
