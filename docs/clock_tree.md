# STM32F405 Clock Tree Architecture

## Overview
The clock system in the STM32F405 controls the operational frequencies for the Cortex-M4 core (SYSCLK) and all peripherals on the AHB and APB buses. The default state after reset runs on an internal 16 MHz RC oscillator (HSI). We configure the clock tree to use an external crystal (HSE) and the internal Phase-Locked Loop (PLL) to hit the chip's maximum clock speed of 168 MHz.

## Clock Flow
1. **HSE (High-Speed External)**: A typical 8 MHz external crystal oscillator.
2. **PLL (Phase-Locked Loop)**:
   - Takes HSE (8 MHz) and divides it by `PLLM=8` to get a 1 MHz VCO input.
   - Multiplies it by `PLLN=336` to get a 336 MHz VCO output.
   - Divides it by `PLLP=2` to output a 168 MHz main clock.
   - Divides by `PLLQ=7` for the 48 MHz USB/SDIO clock.
3. **SYSCLK**: Switched to the 168 MHz PLL output.
4. **AHB (Advanced High-performance Bus)**: Prescaler = 1 (168 MHz). Drives core, memory, GPIOs.
5. **APB1 (Advanced Peripheral Bus 1)**: Prescaler = 4 (42 MHz). Slower peripherals like USART2, TIM2. (Maximum limit is 42 MHz).
6. **APB2 (Advanced Peripheral Bus 2)**: Prescaler = 2 (84 MHz). Faster peripherals like USART1, SPI1. (Maximum limit is 84 MHz).

## Critical Configuration Order
- **Flash Latency**: Flash memory cannot keep up with a 168 MHz CPU. Before increasing the core clock, we *must* configure the Flash Access Control Register (ACR) to insert Wait States (5 WS for 168 MHz at 3.3V) and enable prefetch/caches. Failing to do this causes a HardFault immediately after switching SYSCLK.
- **Bus Prescalers**: APB1 and APB2 prescalers must be set *before* switching SYSCLK to avoid exceeding their hardware frequency limits temporarily.

## Why Clock APIs Matter
Peripheral drivers (like UART or SPI) need precise frequencies to calculate baud rates or timer periods. Since APB1 and APB2 run at different speeds, a generic driver cannot hardcode `168000000`. By exposing `clock_get_pclk1()` and `clock_get_pclk2()`, drivers calculate their timing dynamically, remaining robust to future clock topology changes.
