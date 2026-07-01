#ifndef EVENT_H
#define EVENT_H

#include <stdbool.h>

typedef struct {
    volatile bool signaled;
} event_t;

void event_init(event_t *evt);
void event_set(event_t *evt);
void event_clear(event_t *evt);
void event_wait(event_t *evt);

#endif /* EVENT_H */
