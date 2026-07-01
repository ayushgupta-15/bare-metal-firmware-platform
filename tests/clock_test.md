# Clock Tree Verification

## Methodology
Because QEMU's `netduinoplus2` machine simulates the Cortex-M4 CPU but does not emulate the actual physical timing and gating hardware of the STM32F405 RCC (Reset and Clock Control) registers, true hardware readiness loops (like waiting for `RCC_CR_HSERDY`) may fall through immediately or stall indefinitely in QEMU depending on the emulation accuracy of that specific register state.

To ensure firmware quality, the clock configuration has been verified through **software correctness**:
1. **Compilation**: Cleanly builds with `-Wall -Wextra`.
2. **Timeout Protection**: Production-grade `timeout` decrement loops prevent the initialization from becoming an infinite lockup if a hardware oscillator fails.
3. **Status Codes**: `clock_status_t` strictly defines return codes for error traceability.
4. **Global Getters**: Verification that `system_core_clock`, `pclk1`, and `pclk2` accurately reflect 168 MHz, 42 MHz, and 84 MHz mathematically.

Future debugging on physical silicon will utilize these timeouts if the external 8 MHz crystal oscillator fails to stabilize.
