# STM32F405 Memory Map & Architecture

## Overview
The STM32F405 (Cortex-M4) architecture maps all hardware resources into a single 4GB address space. This includes Flash memory, SRAM, and all peripherals. The core accesses these regions using different buses optimized for different speeds and purposes.

## Memory Layout
| Region | Base Address | Size | Description |
|--------|--------------|------|-------------|
| **FLASH** | `0x08000000` | 512K | Main instruction memory (Code). Contains Vector Table, `.text`, `.rodata`. Read-only during normal execution. |
| **SRAM**  | `0x20000000` | 128K | Main data memory. Contains `.data` (initialized globals), `.bss` (zeroed globals), heap, and stack. |
| **APB1**  | `0x40000000` | — | Low-speed peripheral bus (max 42 MHz). E.g., USART2, USART3, TIM2. |
| **APB2**  | `0x40010000` | — | High-speed peripheral bus (max 84 MHz). E.g., USART1, USART6, TIM1. |
| **AHB1**  | `0x40020000` | — | Core-speed peripheral bus (max 168 MHz). E.g., GPIO A-I, RCC, Flash interface. |
| **Core**  | `0xE0000000` | — | ARM Cortex-M internal peripherals (NVIC, SysTick, SCB). |

## Why AHB and APB?
ARM's Advanced Microcontroller Bus Architecture (AMBA) uses different buses to optimize power and performance:
- **AHB (Advanced High-performance Bus)** connects the core directly to high-speed resources like memory and GPIOs. It runs at the core clock speed.
- **APB (Advanced Peripheral Bus)** connects to slower peripherals like UARTs, I2C, and Timers. It uses a bridge from the AHB to step down the clock speed, reducing power consumption for peripherals that don't need 168 MHz access.

Because APB1 and APB2 run at different maximum speeds, peripheral timing calculations (such as UART Baud Rate Registers) depend entirely on which bus the peripheral is connected to.
