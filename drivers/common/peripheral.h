#ifndef PERIPHERAL_H
#define PERIPHERAL_H

#include "device.h"
#include "status.h"

typedef void* peripheral_t;

status_t peripheral_enable_clock(peripheral_t p);
status_t peripheral_reset(peripheral_t p);
uint32_t peripheral_get_clock(peripheral_t p);

#endif /* PERIPHERAL_H */
