#ifndef STM32F405_H
#define STM32F405_H

#include <stdint.h>
#include "memory_map.h"
#include "core_cm4.h"

/* RCC Register Structure */
typedef struct {
    volatile uint32_t CR;
    volatile uint32_t PLLCFGR;
    volatile uint32_t CFGR;
    volatile uint32_t CIR;
    volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    volatile uint32_t AHB3RSTR;
    uint32_t RESERVED0;
    volatile uint32_t APB1RSTR;
    volatile uint32_t APB2RSTR;
    uint32_t RESERVED1[2];
    volatile uint32_t AHB1ENR;
    volatile uint32_t AHB2ENR;
    volatile uint32_t AHB3ENR;
    uint32_t RESERVED2;
    volatile uint32_t APB1ENR;
    volatile uint32_t APB2ENR;
} RCC_TypeDef;

/* GPIO Register Structure */
typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFR[2];
} GPIO_TypeDef;

/* USART Register Structure */
typedef struct {
    volatile uint32_t SR;
    volatile uint32_t DR;
    volatile uint32_t BRR;
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t GTPR;
} USART_TypeDef;

/* TIM Register Structure */
typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMCR;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t CCMR2;
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
    uint32_t RESERVED0;
    volatile uint32_t CCR1;
    volatile uint32_t CCR2;
    volatile uint32_t CCR3;
    volatile uint32_t CCR4;
    uint32_t RESERVED1;
    volatile uint32_t DCR;
    volatile uint32_t DMAR;
    volatile uint32_t OR;
} TIM_TypeDef;

/* FLASH Register Structure */
typedef struct {
    volatile uint32_t ACR;
    volatile uint32_t KEYR;
    volatile uint32_t OPTKEYR;
    volatile uint32_t SR;
    volatile uint32_t CR;
    volatile uint32_t OPTCR;
} FLASH_TypeDef;

/* Peripheral Definitions */
#define RCC    ((RCC_TypeDef*)    RCC_BASE)

#define GPIOA  ((GPIO_TypeDef*)   GPIOA_BASE)
#define GPIOB  ((GPIO_TypeDef*)   GPIOB_BASE)
#define GPIOC  ((GPIO_TypeDef*)   GPIOC_BASE)
#define GPIOD  ((GPIO_TypeDef*)   GPIOD_BASE)
#define GPIOE  ((GPIO_TypeDef*)   GPIOE_BASE)
#define GPIOF  ((GPIO_TypeDef*)   GPIOF_BASE)
#define GPIOG  ((GPIO_TypeDef*)   GPIOG_BASE)
#define GPIOH  ((GPIO_TypeDef*)   GPIOH_BASE)
#define GPIOI  ((GPIO_TypeDef*)   GPIOI_BASE)

#define USART1 ((USART_TypeDef*)  USART1_BASE)
#define USART2 ((USART_TypeDef*)  USART2_BASE)
#define USART3 ((USART_TypeDef*)  USART3_BASE)
#define USART6 ((USART_TypeDef*)  USART6_BASE)

#define TIM2   ((TIM_TypeDef*)    TIM2_BASE)

#define FLASH  ((FLASH_TypeDef*)  FLASH_R_BASE)

/* RCC Bit Definitions */
#define RCC_AHB1ENR_GPIOAEN (1U << 0)
#define RCC_AHB1ENR_GPIOBEN (1U << 1)
#define RCC_AHB1ENR_GPIOCEN (1U << 2)
#define RCC_AHB1ENR_GPIODEN (1U << 3)
#define RCC_AHB1ENR_GPIOEEN (1U << 4)
#define RCC_AHB1ENR_GPIOFEN (1U << 5)
#define RCC_AHB1ENR_GPIOGEN (1U << 6)
#define RCC_AHB1ENR_GPIOHEN (1U << 7)
#define RCC_AHB1ENR_GPIOIEN (1U << 8)

#define RCC_APB1ENR_TIM2EN   (1U << 0)
#define RCC_APB1ENR_USART2EN (1U << 17)

#define RCC_APB2ENR_USART1EN (1U << 4)

/* RCC CR Bit Definitions */
#define RCC_CR_HSEON         (1U << 16)
#define RCC_CR_HSERDY        (1U << 17)
#define RCC_CR_PLLON         (1U << 24)
#define RCC_CR_PLLRDY        (1U << 25)

/* RCC CFGR Bit Definitions */
#define RCC_CFGR_SW_PLL      (2U << 0)
#define RCC_CFGR_SWS_PLL     (2U << 2)

#define RCC_CFGR_HPRE_DIV1   (0U << 4)
#define RCC_CFGR_PPRE1_DIV4  (5U << 10)
#define RCC_CFGR_PPRE2_DIV2  (4U << 13)

/* RCC PLLCFGR Bit Definitions */
#define RCC_PLLCFGR_PLLSRC_HSE (1U << 22)
#define RCC_PLLCFGR_PLLM_Pos 0
#define RCC_PLLCFGR_PLLN_Pos 6
#define RCC_PLLCFGR_PLLP_Pos 16
#define RCC_PLLCFGR_PLLQ_Pos 24

/* GPIO Bit Definitions */
#define GPIO_MODER_INPUT     0U
#define GPIO_MODER_OUTPUT    1U
#define GPIO_MODER_ALT       2U
#define GPIO_MODER_ANALOG    3U

/* USART Bit Definitions */
#define USART_CR1_UE         (1U << 13)
#define USART_CR1_TXEIE      (1U << 7)
#define USART_CR1_RXNEIE     (1U << 5)
#define USART_CR1_TE         (1U << 3)
#define USART_CR1_RE         (1U << 2)

#define USART_SR_TXE         (1U << 7)
#define USART_SR_TC          (1U << 6)
#define USART_SR_RXNE        (1U << 5)

/* TIM Bit Definitions */
#define TIM_CR1_CEN          (1U << 0)
#define TIM_CR1_OPM          (1U << 3)
#define TIM_CR1_ARPE         (1U << 7)

#define TIM_DIER_UIE         (1U << 0)
#define TIM_SR_UIF           (1U << 0)
#define TIM_EGR_UG           (1U << 0)

/* FLASH Bit Definitions */
#define FLASH_CR_PG          (1U << 0)
#define FLASH_CR_SER         (1U << 1)
#define FLASH_CR_STRT        (1U << 16)
#define FLASH_CR_LOCK        (1U << 31)

#define FLASH_SR_BSY         (1U << 16)

#define FLASH_ACR_LATENCY_5WS  (5U << 0)
#define FLASH_ACR_PRFTEN       (1U << 8)
#define FLASH_ACR_ICEN         (1U << 9)
#define FLASH_ACR_DCEN         (1U << 10)

#endif /* STM32F405_H */
