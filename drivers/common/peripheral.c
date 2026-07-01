#include "peripheral.h"
#include "clock.h"

status_t peripheral_enable_clock(peripheral_t p)
{
    if (p == GPIOA) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; (void)RCC->AHB1ENR; return STATUS_OK; }
    if (p == GPIOB) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; (void)RCC->AHB1ENR; return STATUS_OK; }
    if (p == GPIOC) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; (void)RCC->AHB1ENR; return STATUS_OK; }
    if (p == GPIOD) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN; (void)RCC->AHB1ENR; return STATUS_OK; }
    if (p == GPIOE) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN; (void)RCC->AHB1ENR; return STATUS_OK; }
    if (p == GPIOF) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOFEN; (void)RCC->AHB1ENR; return STATUS_OK; }
    if (p == GPIOG) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN; (void)RCC->AHB1ENR; return STATUS_OK; }
    if (p == GPIOH) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN; (void)RCC->AHB1ENR; return STATUS_OK; }
    if (p == GPIOI) { RCC->AHB1ENR |= RCC_AHB1ENR_GPIOIEN; (void)RCC->AHB1ENR; return STATUS_OK; }

    if (p == USART1) { RCC->APB2ENR |= RCC_APB2ENR_USART1EN; (void)RCC->APB2ENR; return STATUS_OK; }
    if (p == USART2) { RCC->APB1ENR |= RCC_APB1ENR_USART2EN; (void)RCC->APB1ENR; return STATUS_OK; }
    
    if (p == TIM2) { RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; (void)RCC->APB1ENR; return STATUS_OK; }

    return STATUS_UNSUPPORTED;
}

status_t peripheral_reset(peripheral_t p)
{
    /* Not implemented yet */
    (void)p;
    return STATUS_UNSUPPORTED;
}

uint32_t peripheral_get_clock(peripheral_t p)
{
    if (p == GPIOA || p == GPIOB || p == GPIOC || p == GPIOD ||
        p == GPIOE || p == GPIOF || p == GPIOG || p == GPIOH || p == GPIOI) {
        return clock_get_hclk();
    }
    
    if (p == USART1) {
        return clock_get_pclk2(); /* USART1 is on APB2 */
    }

    if (p == USART2) {
        return clock_get_pclk1(); /* USART2 is on APB1 */
    }

    if (p == TIM2) {
        return clock_get_pclk1() * 2; /* TIM2 is on APB1 but multiplied if prescaler > 1 */
    }

    return 0;
}
