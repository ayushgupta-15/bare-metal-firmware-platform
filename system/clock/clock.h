#ifndef CLOCK_H
#define CLOCK_H

#include <stdint.h>

#include "status.h"

status_t clock_init(void);

uint32_t clock_get_sysclk(void);
uint32_t clock_get_hclk(void);
uint32_t clock_get_pclk1(void);
uint32_t clock_get_pclk2(void);

#endif /* CLOCK_H */
