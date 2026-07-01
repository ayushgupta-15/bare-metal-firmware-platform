#ifndef CORE_CM4_H
#define CORE_CM4_H

#include <stdint.h>
#include "memory_map.h"

/* SysTick Register Structure */
typedef struct {
    volatile uint32_t CTRL;  /* Offset: 0x00 */
    volatile uint32_t LOAD;  /* Offset: 0x04 */
    volatile uint32_t VAL;   /* Offset: 0x08 */
    volatile uint32_t CALIB; /* Offset: 0x0C */
} SysTick_TypeDef;

/* SCB Register Structure (partial) */
typedef struct {
    volatile uint32_t CPUID;   /* Offset: 0x00 */
    volatile uint32_t ICSR;    /* Offset: 0x04 */
    volatile uint32_t VTOR;    /* Offset: 0x08 */
    volatile uint32_t AIRCR;   /* Offset: 0x0C */
    volatile uint32_t SCR;     /* Offset: 0x10 */
    volatile uint32_t CCR;     /* Offset: 0x14 */
    volatile uint32_t SHPR[3]; /* Offset: 0x18 */
    volatile uint32_t SHCSR;   /* Offset: 0x24 */
    volatile uint32_t CFSR;    /* Offset: 0x28 */
} SCB_TypeDef;

/* NVIC Register Structure (partial) */
typedef struct {
    volatile uint32_t ISER[8];  /* Offset: 0x000 */
    uint32_t RESERVED0[24];
    volatile uint32_t ICER[8];  /* Offset: 0x080 */
    uint32_t RESERVED1[24];
    volatile uint32_t ISPR[8];  /* Offset: 0x100 */
    uint32_t RESERVED2[24];
    volatile uint32_t ICPR[8];  /* Offset: 0x180 */
    uint32_t RESERVED3[24];
    volatile uint32_t IABR[8];  /* Offset: 0x200 */
    uint32_t RESERVED4[56];
    volatile uint8_t  IP[240];  /* Offset: 0x300 */
} NVIC_TypeDef;

#define SysTick ((SysTick_TypeDef*) SYSTICK_BASE)
#define SCB     ((SCB_TypeDef*)     SCB_BASE)
#define NVIC    ((NVIC_TypeDef*)    NVIC_BASE)

/* SysTick Bit Definitions */
#define SYSTICK_CTRL_ENABLE    (1U << 0)
#define SYSTICK_CTRL_TICKINT   (1U << 1)
#define SYSTICK_CTRL_CLKSOURCE (1U << 2)
#define SYSTICK_CTRL_COUNTFLAG (1U << 16)

#endif /* CORE_CM4_H */
