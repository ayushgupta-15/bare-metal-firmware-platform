# Interrupt-Driven UART & Ring Buffers

## Overview
In the polling implementation (P5), the CPU was forced to wait idly in a `while` loop for the UART hardware's `TXE` (Transmit Data Register Empty) flag to assert before sending the next byte. At 115200 baud, sending a 100-byte log message wastes nearly 100,000 CPU cycles. 

By migrating to an interrupt-driven model, we drastically improve system throughput. The application simply writes its payload to a memory-backed Ring Buffer and immediately returns to normal execution. The UART peripheral's hardware interrupts (`USART1_IRQHandler`) handle ferrying bytes from the memory buffer to the physical shift register in the background.

## Ring Buffer Mechanics
A ring buffer (circular queue) decouples the Producer (the application) from the Consumer (the ISR).
- It uses a `head` index (where new data is written) and a `tail` index (where data is read).
- As long as the `head` and `tail` pointers do not overlap (meaning full or empty), data flows seamlessly.
- **Lock-free design**: For a Single Producer, Single Consumer (SPSC) setup, ring buffers can operate without mutexes. However, because `uart_write_async` might be called by multiple application threads (or preempted), pushing to the buffer is wrapped in a `critical_enter()`/`critical_exit()` block to prevent pointer corruption.

## ISR State Machine
The `USART1_IRQHandler` manages two primary flags:
1. **RXNE (Receive Not Empty)**: Fired when a byte arrives. The ISR reads `DR` and pushes it into the RX ring buffer.
2. **TXE (Transmit Empty)**: Fired when the hardware is ready for the next byte. The ISR pops from the TX ring buffer and writes to `DR`.

### Disabling TXE
Crucially, if the TX ring buffer is *empty*, the ISR **must** disable the `TXEIE` (TXE Interrupt Enable) bit in `CR1`. If it doesn't, the `TXE` flag remains asserted (since `DR` is empty), and the CPU will endlessly re-enter the ISR, completely locking up the system (interrupt storm). `TXEIE` is subsequently re-enabled by the application whenever new data is written to the buffer.

## Logging Backend Abstraction
Instead of hardcoding UART calls inside `log.c`, the logging module now relies on a generic `log_backend_t` function pointer struct. This decouples the core firmware's logging from the physical transport. Tomorrow, if we want to log over USB CDC or SWO, we simply instantiate a different backend struct and pass it to `log_init()`, requiring exactly zero changes to the underlying `log_info()` API. This is the cornerstone of professional BSP design.
