#include "gpio.h"
#include "peripheral.h"

status_t gpio_init(GPIO_TypeDef *port, uint16_t pin_mask, gpio_mode_t mode, 
                   gpio_output_type_t out_type, gpio_pull_t pull, gpio_speed_t speed)
{
    if (!port || pin_mask == 0) {
        return STATUS_INVALID_PARAM;
    }

    peripheral_enable_clock(port);

    for (uint32_t i = 0; i < 16; i++) {
        if (pin_mask & (1U << i)) {
            /* Configure Mode */
            port->MODER &= ~(3U << (i * 2));
            port->MODER |= ((uint32_t)mode << (i * 2));

            /* Configure Output Type */
            port->OTYPER &= ~(1U << i);
            port->OTYPER |= ((uint32_t)out_type << i);

            /* Configure Speed */
            port->OSPEEDR &= ~(3U << (i * 2));
            port->OSPEEDR |= ((uint32_t)speed << (i * 2));

            /* Configure Pull-up/Pull-down */
            port->PUPDR &= ~(3U << (i * 2));
            port->PUPDR |= ((uint32_t)pull << (i * 2));
        }
    }

    return STATUS_OK;
}

status_t gpio_write(GPIO_TypeDef *port, uint16_t pin_mask, bool state)
{
    if (!port) return STATUS_INVALID_PARAM;

    if (state) {
        /* BSRR lower 16 bits set pins */
        port->BSRR = pin_mask;
    } else {
        /* BSRR upper 16 bits reset pins */
        port->BSRR = ((uint32_t)pin_mask << 16);
    }

    return STATUS_OK;
}

status_t gpio_toggle(GPIO_TypeDef *port, uint16_t pin_mask)
{
    if (!port) return STATUS_INVALID_PARAM;

    /* To toggle atomically with respect to other pins, read ODR and write BSRR */
    uint32_t odr = port->ODR;
    port->BSRR = ((odr & pin_mask) << 16) | (~odr & pin_mask);

    return STATUS_OK;
}

bool gpio_read(const GPIO_TypeDef *port, uint16_t pin)
{
    if (!port) return false;
    
    return (port->IDR & pin) != 0;
}

status_t gpio_set_af(GPIO_TypeDef *port, uint16_t pin_mask, uint8_t af)
{
    if (!port) return STATUS_INVALID_PARAM;

    for (uint32_t i = 0; i < 16; i++) {
        if (pin_mask & (1U << i)) {
            uint32_t afr_idx = i >> 3; /* 0 for 0-7, 1 for 8-15 */
            uint32_t af_pos = (i & 7) * 4;
            port->AFR[afr_idx] &= ~(0xFU << af_pos);
            port->AFR[afr_idx] |= ((uint32_t)af << af_pos);
        }
    }
    return STATUS_OK;
}
