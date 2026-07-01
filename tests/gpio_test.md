# GPIO Verification

## Methodology
Because QEMU's `netduinoplus2` board emulation does not connect the STM32F405 GPIO peripherals to an interactive physical environment (like a terminal window or GUI LEDs), verifying GPIO purely in software involves asserting register manipulations and ensuring execution flow.

## Verification Checklist

1. **Initialization API**
   - The `gpio_init()` function must reject `NULL` ports or `0` masks by returning `STATUS_INVALID_PARAM`.
   - The driver must autonomously trigger `RCC->AHB1ENR` for the correct peripheral bus (e.g., `GPIOAEN` for `GPIOA`) before touching configuration registers.

2. **Register Writes**
   - The `gpio_write()` function uses the atomic `BSRR` register exclusively.
   - Asserted that `state = true` writes to the lower 16 bits.
   - Asserted that `state = false` writes to the upper 16 bits.

3. **Atomic Toggle**
   - Implemented an interrupt-safe toggle via reading `ODR` and writing the toggled state purely through `BSRR`. This preserves atomicity with respect to other pins on the same port.

4. **Integration**
   - `main.c` successfully configures an input (PC13) and output (PA5), reading the input via `gpio_read()` and modulating the output via `gpio_write()` and `gpio_toggle()`.
   - Compiled with `-Wall -Wextra` yielding zero warnings, demonstrating type safety with the provided enums.
