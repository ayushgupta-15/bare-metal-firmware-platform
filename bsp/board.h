#ifndef BOARD_H
#define BOARD_H

#include "status.h"
#include "callback.h"

status_t board_init(callback_t timer_cb, void *timer_arg);

#endif /* BOARD_H */
