# General Purpose Timer (TIM2)

## Overview
While `SysTick` provides a central, 1ms timebase for delays and RTOS scheduling, General Purpose Timers like `TIM2` give the application highly precise, independent hardware schedulers. They can be used for microsecond-level timeouts, generating PWM waves, or capturing input timings without burdening the core processor.

## Hardware Components
- **PSC (Prescaler)**: Divides the input clock (`PCLK1` * 2, typically 84MHz on STM32F4) to achieve a desired counting frequency. For example, dividing 84MHz by 84 yields a 1MHz clock, meaning each "tick" of the timer takes exactly 1 microsecond.
- **ARR (Auto-Reload Register)**: Determines the maximum value the counter (`CNT`) can reach. When `CNT` equals `ARR`, an "Update Event" occurs. In periodic mode, `CNT` resets to 0 and starts counting again. In one-shot mode (`OPM` bit), the timer halts.
- **UIF (Update Interrupt Flag)**: Asserted by hardware when an Update Event occurs. If the `UIE` (Update Interrupt Enable) bit is set, this triggers the `TIM2_IRQHandler`.
- **UG (Update Generation)**: A software trigger in the `EGR` register that forces an immediate Update Event. This is extremely useful to immediately apply changes written to the shadow registers (like PSC and ARR) rather than waiting for the previous countdown to finish.

## Software Architecture
To maximize reusability across different peripherals (SPI, I2C, DMA), we utilize a **Configuration Object (`timer_config_t`)**. Instead of passing 5 discrete arguments into `timer_init()`, a single configuration struct defines the timer instance, frequency, operating mode, and the ISR callback function.

### Callbacks vs. Polling
When the timer expires, the hardware invokes `TIM2_IRQHandler`. Instead of writing monolithic logic directly into the ISR (which breaks layering), the driver maintains a `timer_callback_t` function pointer. When the IRQ fires, it clears the `UIF` hardware flag and immediately invokes the user's registered callback. This allows the application logic to cleanly decouple from the bare-metal exception handling.
