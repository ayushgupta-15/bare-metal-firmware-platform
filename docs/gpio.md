# GPIO Driver Architecture

## Overview
The GPIO (General Purpose Input/Output) driver abstract the STM32F405 register manipulation into a clean, enumerator-driven API. It strictly prohibits hardcoded memory addresses inside application code, enforcing isolation through the `device.h` CMSIS-style definitions.

## Register Layout
Each GPIO port (e.g., `GPIOA`, `GPIOB`) controls 16 independent pins via a set of 32-bit registers:
- **MODER** (Mode Register): 2 bits per pin. Configures Input (00), Output (01), Alternate Function (10), or Analog (11).
- **OTYPER** (Output Type Register): 1 bit per pin. Push-Pull (0) or Open-Drain (1).
- **OSPEEDR** (Output Speed Register): 2 bits per pin. Controls the slew rate of the output signal (Low, Medium, Fast, High).
- **PUPDR** (Pull-up/Pull-down Register): 2 bits per pin. Connects internal pull resistors: None (00), Pull-up (01), Pull-down (10).
- **IDR** (Input Data Register): Read-only, 1 bit per pin. Reflects the actual electrical state of the pin.
- **ODR** (Output Data Register): Read/write, 1 bit per pin. Sets the target output state, but modifying a single pin requires a read-modify-write operation.
- **BSRR** (Bit Set/Reset Register): Write-only. The preferred mechanism for changing pin states atomically.

## Why BSRR instead of ODR?
A common interview question is why writing to `ODR` is discouraged for setting pins high/low. 
If you write `GPIOA->ODR |= (1 << 5);`, the CPU executes three instructions: 
1. Read `ODR`
2. Bitwise OR with `(1 << 5)`
3. Write back to `ODR`

If an interrupt occurs between steps 1 and 3 that modifies another pin on `GPIOA`, step 3 will blindly overwrite the interrupt's change because it uses stale data from step 1. This is a classic race condition.

The **BSRR** (Bit Set/Reset Register) solves this elegantly in hardware. Writing a `1` to the lower 16 bits *sets* the corresponding pin. Writing a `1` to the upper 16 bits *resets* (clears) the pin. Writing a `0` does nothing. This allows a single, atomic store instruction to change a pin's state without affecting or even knowing the state of any other pin.

```c
// Atomic Set (Pin 5)
GPIOA->BSRR = (1 << 5);

// Atomic Reset (Pin 5)
GPIOA->BSRR = (1 << (5 + 16));
```
