# UART Driver Architecture

## Overview
The Universal Asynchronous Receiver-Transmitter (UART) is the primary debugging and communication interface for the firmware. This driver currently implements blocking (polling) transmission and reception. 

## Baud Rate Calculation
The baud rate is derived from the peripheral clock driving the USART block.
On the STM32F405:
- **USART1 and USART6** are on **APB2** (PCLK2, max 84 MHz)
- **USART2, USART3, UART4, UART5** are on **APB1** (PCLK1, max 42 MHz)

Instead of hardcoding a baud rate register (BRR) value, the driver uses the generic `peripheral_get_clock()` API.
Assuming 16x oversampling (`OVER8=0`), the equation is:
`USARTDIV = f_CK / (16 * baud)`
The mantissa goes into BRR[15:4] and fraction into BRR[3:0]. Conveniently, shifting the fraction up by 4 makes the final BRR value exactly `f_CK / baud` (with `+ baud / 2` added for integer rounding).

## TXE vs TC Flags
There are two critical status flags in the `USART_SR` register for transmission:
1. **TXE (Transmit Data Register Empty)**: This bit is set when the `DR` (Data Register) has transferred its contents into the internal shift register. You can write the *next* byte to `DR` immediately when `TXE` is 1, even if the current byte is still being transmitted physically over the TX pin. This enables back-to-back byte streaming.
2. **TC (Transmission Complete)**: This bit is set only when the shift register is completely empty and no more bytes are transmitting. This is useful for `uart_flush()`, which must guarantee that all bits have physically left the pin before doing something like disabling the clock or going to sleep.

## Polling Inefficiency & Foreshadowing
Currently, `uart_write_byte` uses a `while(!(SR & TXE))` loop. Even with timeouts, this forces the CPU to wait hundreds or thousands of cycles doing absolutely nothing while the UART hardware shifts bits out. For a 168 MHz CPU, waiting for a 115200 baud transmission wastes massive amounts of processing power. 

In a future phase (P7), this will be upgraded to an interrupt-driven architecture using a ring buffer. The application will immediately push strings into a memory buffer and return, while an ISR handles feeding bytes to the `DR` register asynchronously in the background.
