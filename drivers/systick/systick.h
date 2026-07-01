#ifndef SYSTICK_H
#define SYSTICK_H

#include "device.h"
#include "status.h"
#include <stdint.h>

status_t systick_init(uint32_t tick_hz);
uint32_t systick_get_ticks(void);
void delay_ms(uint32_t ms);

#endif /* SYSTICK_H */
