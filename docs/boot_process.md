# Cortex-M4 Boot Process

## 1. Reset Sequence
Upon power-on or reset, the ARM Cortex-M core performs the following exact sequence:
1. It reads the 32-bit word at memory address `0x00000000`. This value is loaded into the **Main Stack Pointer (MSP)**.
2. It reads the 32-bit word at memory address `0x00000004`. This value is the address of the **Reset_Handler**, which is loaded into the **Program Counter (PC)**.
3. Execution begins at `Reset_Handler`.

On STM32 devices, the Flash memory at `0x08000000` is aliased to `0x00000000` via hardware during normal boot, meaning the core is reading the very beginning of our Flash memory.

## 2. Vector Table
The vector table is an array of function pointers placed at the very start of Flash (`.isr_vector` section).
- **Position 0**: Initial Stack Pointer (`_estack` defined in linker script).
- **Position 1**: `Reset_Handler`.
- **Positions 2-15**: Core Exceptions (NMI, HardFault, SysTick, etc.).
- **Positions 16+**: Peripheral Interrupts (EXTI, UART, SPI, etc.).

## 3. Reset_Handler Responsibilities
Our `Reset_Handler` (in `startup_arm.s`) does the fundamental setup required for a C environment:

### `copy .data`
Global/static variables that are initialized with non-zero values (e.g., `int x = 5;`) are placed in the `.data` section. Because RAM is volatile and loses its state when powered off, these initial values must be stored in Flash memory. During boot, `Reset_Handler` copies them from their Load Memory Address (LMA) in Flash to their Virtual Memory Address (VMA) in RAM.

### `zero .bss`
Global/static variables that are uninitialized or explicitly initialized to zero (e.g., `int y;`) are placed in the `.bss` section. The C standard dictates these must be zeroed before `main()` runs. `Reset_Handler` clears this RAM region.

### `SystemInit()`
Called after memory setup. Its purpose is to configure the core clock tree, PLLs, and Flash wait states. This prepares the hardware to run at full speed before complex application logic begins.

### `main()`
Finally, it branches to the user's `main()` function, where the high-level application begins.
