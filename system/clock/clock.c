#include "clock.h"
#include "device.h"

/* Clock state variables */
static uint32_t system_core_clock = 16000000; /* Default HSI */
static uint32_t hclk = 16000000;
static uint32_t pclk1 = 16000000;
static uint32_t pclk2 = 16000000;

#define TIMEOUT_MAX 0x100000U

status_t clock_init(void)
{
    uint32_t timeout;

    /* 1. Enable HSE */
    RCC->CR |= RCC_CR_HSEON;
    timeout = TIMEOUT_MAX;
    while (!(RCC->CR & RCC_CR_HSERDY)) {
        if (--timeout == 0) {
            return STATUS_TIMEOUT;
        }
    }

    /* 2. Configure Flash Latency */
    /* For 168MHz at 3.3V, 5 wait states are required. Also enable prefetch, I-cache, D-cache */
    FLASH->ACR = FLASH_ACR_LATENCY_5WS | FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN;

    /* 3. Configure PLL */
    /* Target: 168 MHz from 8 MHz HSE
       PLLM = 8   (VCO input = HSE / 8 = 1 MHz)
       PLLN = 336 (VCO output = 1 MHz * 336 = 336 MHz)
       PLLP = 2   (PLLCLK = VCO / 2 = 168 MHz)
       PLLQ = 7   (USB/SDIO/RNG clock = 336 / 7 = 48 MHz) 
       Note: PLLP value in register is (PLLP/2 - 1). For PLLP=2, write 0. */
    RCC->PLLCFGR = (8U << RCC_PLLCFGR_PLLM_Pos) |
                   (336U << RCC_PLLCFGR_PLLN_Pos) |
                   (0U << RCC_PLLCFGR_PLLP_Pos) |
                   (7U << RCC_PLLCFGR_PLLQ_Pos) |
                   RCC_PLLCFGR_PLLSRC_HSE;

    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;
    timeout = TIMEOUT_MAX;
    while (!(RCC->CR & RCC_CR_PLLRDY)) {
        if (--timeout == 0) {
            return STATUS_TIMEOUT;
        }
    }

    /* 4. Configure Bus Prescalers */
    /* AHB (HCLK) = SYSCLK / 1 = 168 MHz
       APB1 (PCLK1) = HCLK / 4 = 42 MHz (Max 42MHz)
       APB2 (PCLK2) = HCLK / 2 = 84 MHz (Max 84MHz) */
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1 | RCC_CFGR_PPRE1_DIV4 | RCC_CFGR_PPRE2_DIV2;

    /* 5. Switch SYSCLK to PLL */
    RCC->CFGR = (RCC->CFGR & ~3U) | RCC_CFGR_SW_PLL; /* 3U is SW mask */

    /* Wait until PLL is used as system clock source */
    timeout = TIMEOUT_MAX;
    while ((RCC->CFGR & 12U) != RCC_CFGR_SWS_PLL) { /* 12U is SWS mask (0xC) */
        if (--timeout == 0) {
            return STATUS_TIMEOUT;
        }
    }

    /* 6. Update global clock variables */
    system_core_clock = 168000000;
    hclk = system_core_clock;
    pclk1 = hclk / 4;
    pclk2 = hclk / 2;

    return STATUS_OK;
}

uint32_t clock_get_sysclk(void) { return system_core_clock; }
uint32_t clock_get_hclk(void)   { return hclk; }
uint32_t clock_get_pclk1(void)  { return pclk1; }
uint32_t clock_get_pclk2(void)  { return pclk2; }
