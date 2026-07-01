#include "systick.h"
#include "clock.h"
#include "nvic.h"
#include "critical.h"

static volatile uint32_t system_ticks = 0;
static uint32_t current_tick_hz = 1000; /* Default 1 ms */

void SysTick_Handler(void)
{
    system_ticks++;
}

status_t systick_init(uint32_t tick_hz)
{
    if (tick_hz == 0) return STATUS_INVALID_PARAM;

    current_tick_hz = tick_hz;
    uint32_t sysclk = clock_get_sysclk();
    
    uint32_t reload = (sysclk / tick_hz) - 1;
    if (reload > 0x00FFFFFF) {
        /* SysTick is a 24-bit timer */
        return STATUS_INVALID_PARAM;
    }

    SysTick->LOAD = reload;
    SysTick->VAL = 0;

    /* Set SysTick interrupt priority to lowest (15 for 4-bit priority) 
       SysTick IRQn is -1 */
    nvic_set_priority(-1, 15);

    /* Enable SysTick, use core clock, enable interrupt */
    SysTick->CTRL = SYSTICK_CTRL_ENABLE | SYSTICK_CTRL_TICKINT | SYSTICK_CTRL_CLKSOURCE;

    return STATUS_OK;
}

uint32_t systick_get_ticks(void)
{
    uint32_t ticks;
    critical_enter();
    ticks = system_ticks;
    critical_exit();
    return ticks;
}

void delay_ms(uint32_t ms)
{
    /* Convert ms to ticks based on current_tick_hz */
    uint32_t delay_ticks = (ms * current_tick_hz) / 1000;
    uint32_t start = systick_get_ticks();

    /* 32-bit unsigned arithmetic handles wraparound natively */
    while ((systick_get_ticks() - start) < delay_ticks) {
        /* Wait */
    }
}
