#include "critical.h"
#include <stdint.h>

static uint32_t saved_primask = 0;
static uint32_t nest_count = 0;

void critical_enter(void)
{
    uint32_t primask;
    /* Read the current PRIMASK value */
    __asm volatile("mrs %0, primask" : "=r" (primask) :: "memory");
    /* Disable interrupts globally */
    __asm volatile("cpsid i" : : : "memory");

    if (nest_count == 0) {
        saved_primask = primask;
    }
    nest_count++;
}

void critical_exit(void)
{
    if (nest_count > 0) {
        nest_count--;
        if (nest_count == 0) {
            /* Restore the original PRIMASK value */
            __asm volatile("msr primask, %0" :: "r" (saved_primask) : "memory");
        }
    }
}
