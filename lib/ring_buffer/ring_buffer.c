#include "ring_buffer.h"

void ring_buffer_init(ring_buffer_t *rb, uint8_t *buffer, size_t size)
{
    if (!rb || !buffer || size == 0) return;
    rb->buffer = buffer;
    rb->size = size;
    rb->head = 0;
    rb->tail = 0;
}

bool ring_buffer_push(ring_buffer_t *rb, uint8_t data)
{
    if (ring_buffer_is_full(rb)) {
        return false;
    }
    
    rb->buffer[rb->head] = data;
    rb->head = (rb->head + 1) % rb->size;
    return true;
}

bool ring_buffer_pop(ring_buffer_t *rb, uint8_t *data)
{
    if (ring_buffer_is_empty(rb)) {
        return false;
    }
    
    *data = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) % rb->size;
    return true;
}

bool ring_buffer_is_empty(const ring_buffer_t *rb)
{
    return rb->head == rb->tail;
}

bool ring_buffer_is_full(const ring_buffer_t *rb)
{
    return ((rb->head + 1) % rb->size) == rb->tail;
}
