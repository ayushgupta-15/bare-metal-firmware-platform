#ifndef MEMORY_MAP_H
#define MEMORY_MAP_H

/* Core Memory Regions */
#define FLASH_BASE        0x08000000U
#define SRAM_BASE         0x20000000U

/* Peripheral Bus Bases */
#define APB1PERIPH_BASE   0x40000000U
#define APB2PERIPH_BASE   0x40010000U
#define AHB1PERIPH_BASE   0x40020000U
#define AHB2PERIPH_BASE   0x50000000U

/* AHB1 Peripherals */
#define GPIOA_BASE        (AHB1PERIPH_BASE + 0x0000U)
#define GPIOB_BASE        (AHB1PERIPH_BASE + 0x0400U)
#define GPIOC_BASE        (AHB1PERIPH_BASE + 0x0800U)
#define GPIOD_BASE        (AHB1PERIPH_BASE + 0x0C00U)
#define GPIOE_BASE        (AHB1PERIPH_BASE + 0x1000U)
#define GPIOF_BASE        (AHB1PERIPH_BASE + 0x1400U)
#define GPIOG_BASE        (AHB1PERIPH_BASE + 0x1800U)
#define GPIOH_BASE        (AHB1PERIPH_BASE + 0x1C00U)
#define GPIOI_BASE        (AHB1PERIPH_BASE + 0x2000U)

#define RCC_BASE          (AHB1PERIPH_BASE + 0x3800U)
#define FLASH_R_BASE      (AHB1PERIPH_BASE + 0x3C00U)

/* APB1 Peripherals */
#define TIM2_BASE         (APB1PERIPH_BASE + 0x0000U)
#define SPI2_BASE         (APB1PERIPH_BASE + 0x3800U)
#define SPI3_BASE         (APB1PERIPH_BASE + 0x3C00U)
#define USART2_BASE       (APB1PERIPH_BASE + 0x4400U)
#define USART3_BASE       (APB1PERIPH_BASE + 0x4800U)
#define UART4_BASE        (APB1PERIPH_BASE + 0x4C00U)
#define UART5_BASE        (APB1PERIPH_BASE + 0x5000U)
#define I2C1_BASE         (APB1PERIPH_BASE + 0x5400U)
#define I2C2_BASE         (APB1PERIPH_BASE + 0x5800U)
#define I2C3_BASE         (APB1PERIPH_BASE + 0x5C00U)

/* APB2 Peripherals */
#define TIM1_BASE         (APB2PERIPH_BASE + 0x0000U)
#define USART1_BASE       (APB2PERIPH_BASE + 0x1000U)
#define USART6_BASE       (APB2PERIPH_BASE + 0x1400U)
#define SPI1_BASE         (APB2PERIPH_BASE + 0x3000U)

/* Core Peripherals */
#define SCS_BASE          0xE000E000U
#define SYSTICK_BASE      (SCS_BASE + 0x0010U)
#define NVIC_BASE         (SCS_BASE + 0x0100U)
#define SCB_BASE          (SCS_BASE + 0x0D00U)

#endif /* MEMORY_MAP_H */
