#ifndef TIMER_H
#define TIMER_H

#include "device.h"
#include "status.h"
#include "callback.h"
#include <stdint.h>

typedef enum {
    TIMER_MODE_ONE_SHOT = 0,
    TIMER_MODE_PERIODIC
} timer_mode_t;

typedef struct {
    TIM_TypeDef *timer;
    uint32_t frequency;      /* Target tick frequency of the timer in Hz (e.g., 1000000 for 1us tick) */
    timer_mode_t mode;
    callback_t callback;
    void *callback_arg;
} timer_config_t;

status_t timer_init(const timer_config_t *config);
status_t timer_start(TIM_TypeDef *timer);
status_t timer_stop(TIM_TypeDef *timer);
status_t timer_set_period(TIM_TypeDef *timer, uint32_t us);

#endif /* TIMER_H */
