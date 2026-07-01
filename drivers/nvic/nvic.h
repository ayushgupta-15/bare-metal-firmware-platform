#ifndef NVIC_H
#define NVIC_H

#include "device.h"
#include "status.h"

status_t nvic_enable_irq(int32_t irq);
status_t nvic_disable_irq(int32_t irq);
status_t nvic_set_priority(int32_t irq, uint32_t priority);

#endif /* NVIC_H */
