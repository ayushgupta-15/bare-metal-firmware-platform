#ifndef GPIO_H
#define GPIO_H

#include "device.h"
#include "status.h"
#include <stdbool.h>

typedef enum {
    GPIO_MODE_INPUT = 0,
    GPIO_MODE_OUTPUT,
    GPIO_MODE_ALTERNATE,
    GPIO_MODE_ANALOG
} gpio_mode_t;

typedef enum {
    GPIO_OUTPUT_PUSHPULL = 0,
    GPIO_OUTPUT_OPENDRAIN
} gpio_output_type_t;

typedef enum {
    GPIO_SPEED_LOW = 0,
    GPIO_SPEED_MEDIUM,
    GPIO_SPEED_FAST,
    GPIO_SPEED_HIGH
} gpio_speed_t;

typedef enum {
    GPIO_PULL_NONE = 0,
    GPIO_PULL_UP,
    GPIO_PULL_DOWN
} gpio_pull_t;

#define GPIO_PIN_0   (1U << 0)
#define GPIO_PIN_1   (1U << 1)
#define GPIO_PIN_2   (1U << 2)
#define GPIO_PIN_3   (1U << 3)
#define GPIO_PIN_4   (1U << 4)
#define GPIO_PIN_5   (1U << 5)
#define GPIO_PIN_6   (1U << 6)
#define GPIO_PIN_7   (1U << 7)
#define GPIO_PIN_8   (1U << 8)
#define GPIO_PIN_9   (1U << 9)
#define GPIO_PIN_10  (1U << 10)
#define GPIO_PIN_11  (1U << 11)
#define GPIO_PIN_12  (1U << 12)
#define GPIO_PIN_13  (1U << 13)
#define GPIO_PIN_14  (1U << 14)
#define GPIO_PIN_15  (1U << 15)
#define GPIO_PIN_ALL 0xFFFFU

status_t gpio_init(GPIO_TypeDef *port, uint16_t pin_mask, gpio_mode_t mode, 
                   gpio_output_type_t out_type, gpio_pull_t pull, gpio_speed_t speed);

status_t gpio_write(GPIO_TypeDef *port, uint16_t pin_mask, bool state);
status_t gpio_toggle(GPIO_TypeDef *port, uint16_t pin_mask);
bool gpio_read(const GPIO_TypeDef *port, uint16_t pin);
status_t gpio_set_af(GPIO_TypeDef *port, uint16_t pin_mask, uint8_t af);

#endif /* GPIO_H */
