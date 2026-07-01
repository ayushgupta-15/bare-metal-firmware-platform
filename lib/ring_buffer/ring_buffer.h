#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct {
    uint8_t *buffer;
    size_t size;
    volatile size_t head; /* Write index */
    volatile size_t tail; /* Read index */
} ring_buffer_t;

void ring_buffer_init(ring_buffer_t *rb, uint8_t *buffer, size_t size);
bool ring_buffer_push(ring_buffer_t *rb, uint8_t data);
bool ring_buffer_pop(ring_buffer_t *rb, uint8_t *data);
bool ring_buffer_is_empty(const ring_buffer_t *rb);
bool ring_buffer_is_full(const ring_buffer_t *rb);

#endif /* RING_BUFFER_H */
