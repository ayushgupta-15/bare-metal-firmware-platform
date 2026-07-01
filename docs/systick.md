# SysTick & NVIC Architecture

## Overview
The System Tick Timer (SysTick) is a core peripheral included in nearly all ARM Cortex-M processors. It provides a simple, deterministic 24-bit down-counter that triggers an exception (`SysTick_Handler`) when it reaches zero. This forms the heartbeat of modern embedded platforms and RTOS kernels.

## Hardware Mechanism
1. **LOAD Register**: We calculate the reload value based on the core clock (SYSCLK) to achieve a precise 1 ms interval: `Reload = (SYSCLK / 1000) - 1`. 
2. **VAL Register**: The current counter value. When it hits `0`, it reloads from `LOAD` on the next clock cycle and sets the `COUNTFLAG`.
3. **Interrupts**: The timer triggers exception 15. The `SysTick_Handler` fires and increments a global `system_ticks` variable.

## Why use SysTick over Software Loops?
A naive delay loop `for (int i = 0; i < 100000; i++) asm("nop");` executes at a rate entirely dependent on the CPU frequency and compiler optimization level. If we switch the clock from 16 MHz to 168 MHz, a software delay becomes 10x faster and breaks timing. Using SysTick guarantees that `delay_ms(100)` is exactly 100 milliseconds regardless of core speed, wait states, or compiler flags.

## Overflow Handling
A 32-bit `system_ticks` counter tracking milliseconds will overflow and wrap back to `0` after approximately 49.7 days.
If we implement a delay as: `while(current_tick < start_tick + delay);`, an overflow on `start_tick + delay` will cause the loop to exit immediately.
Instead, we use modular arithmetic: `while((current_tick - start_tick) < delay);`. In 32-bit unsigned arithmetic, `(2 - 0xFFFFFFFF) == 3`, correctly reflecting that 3 ticks have elapsed, thereby gracefully surviving wraparound.

## Critical Sections & PRIMASK
Shared state (like `system_ticks`) accessed in both ISRs and Thread mode introduces concurrency risks. A `critical_enter()` macro disables global interrupts by writing to the `PRIMASK` register via the `cpsid i` instruction. It restores the previous state on `critical_exit()`. This completely eliminates data races when reading multi-byte variables on 8-bit platforms or executing complex read-modify-write operations on 32-bit platforms.

## NVIC and SCB
The NVIC (Nested Vectored Interrupt Controller) governs external peripheral interrupts. However, core exceptions like SysTick and PendSV are managed via the System Control Block (SCB). Our `nvic_set_priority` abstraction elegantly bridges this by calculating whether an IRQ number maps to the NVIC `IP` array or the SCB `SHPR` array, allowing uniform priority management for the entire platform.
