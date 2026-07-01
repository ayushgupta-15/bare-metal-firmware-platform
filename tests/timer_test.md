# Timer Driver Verification

## Test Overview
The objective is to verify that the General Purpose Timer (TIM2) accurately triggers hardware interrupts at a configured interval, successfully invoking the user-registered callback to output telemetry via the UART backend.

## Expected Behavior
1. The firmware boots and initializes the core clock (PCLK1).
2. The UART is configured and the logger is routed through the asynchronous ring buffer.
3. The application sets up `TIM2` with a `timer_config_t` object:
   - Target frequency: `10000 Hz` (ticks every 100 µs).
   - Mode: `TIMER_MODE_PERIODIC`.
   - Callback: `timer_callback()`.
4. The application starts the timer with a period of `5000` (representing 500 ms).
5. The CPU drops into an infinite `while(1)` sleep loop.
6. Twice per second, the `TIM2_IRQHandler` fires, clears the hardware flag, and invokes `timer_callback()`.
7. `timer_callback()` pushes `"Timer Event"` into the UART TX ring buffer.
8. The UART ISR asynchronously flushes the payload to the virtual console.

## QEMU Validation
Run `make clean all && make run_qemu`.

**Expected Output:**
```
[INFO] Booting...
[INFO] Clock Initialized
[INFO] GPIO Initialized
[INFO] UART Initialized
[INFO] Interrupt-driven UART Active
[INFO] Timer Initialized and Started (500ms)
[INFO] Timer Event
[INFO] Timer Event
[INFO] Timer Event
...
```
