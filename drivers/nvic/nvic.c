#include "nvic.h"

status_t nvic_enable_irq(int32_t irq)
{
    if (irq < 0) {
        /* Core exceptions are always active or enabled via SCB, not NVIC ISER */
        return STATUS_INVALID_PARAM;
    }
    NVIC->ISER[irq >> 5] = (1UL << (irq & 0x1F));
    return STATUS_OK;
}

status_t nvic_disable_irq(int32_t irq)
{
    if (irq < 0) {
        return STATUS_INVALID_PARAM;
    }
    NVIC->ICER[irq >> 5] = (1UL << (irq & 0x1F));
    return STATUS_OK;
}

status_t nvic_set_priority(int32_t irq, uint32_t priority)
{
    /* Cortex-M4 typically implements 4 bits for priority (upper 4 bits of the byte) */
    uint8_t prio_val = (uint8_t)((priority << 4) & 0xFF);

    if (irq < 0) {
        /* Core exceptions: 
           Exception number = irq + 16.
           Exceptions 4-15 are in SHPR[0..2].
        */
        int32_t exc = irq + 16;
        if (exc >= 4 && exc < 16) {
            uint32_t idx = (exc / 4) - 1;
            uint32_t shift = (exc % 4) * 8;
            uint32_t val = SCB->SHPR[idx];
            val &= ~(0xFFUL << shift);
            val |= ((uint32_t)prio_val << shift);
            SCB->SHPR[idx] = val;
        } else {
            return STATUS_INVALID_PARAM;
        }
    } else {
        /* Peripheral interrupts */
        NVIC->IP[irq] = prio_val;
    }
    
    return STATUS_OK;
}
