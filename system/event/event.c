#include "event.h"

void event_init(event_t *evt)
{
    if (evt) {
        evt->signaled = false;
    }
}

void event_set(event_t *evt)
{
    if (evt) {
        evt->signaled = true;
    }
}

void event_clear(event_t *evt)
{
    if (evt) {
        evt->signaled = false;
    }
}

void event_wait(event_t *evt)
{
    if (evt) {
        /* Currently busy-waits. Will map to RTOS block API later. */
        while (!evt->signaled) {
            /* Wait */
        }
    }
}
