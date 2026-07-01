/* linker/stm32f405.ld */
MEMORY
{
  FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 512K
  RAM (xrw)  : ORIGIN = 0x20000000, LENGTH = 128K
}

ENTRY(Reset_Handler)

SECTIONS
{
  .text :
  {
    . = ALIGN(4);
    *(.isr_vector)
    *(.text)
    *(.text*)
    *(.rodata)
    *(.rodata*)
    . = ALIGN(4);
  } > FLASH

  _sidata = LOADADDR(.data);

  .data :
  {
    . = ALIGN(4);
    _sdata = .;
    *(.data)
    *(.data*)
    . = ALIGN(4);
    _edata = .;
  } > RAM AT> FLASH

  .bss :
  {
    . = ALIGN(4);
    _sbss = .;
    *(.bss)
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
  } > RAM

  .heap (NOLOAD) :
  {
    . = ALIGN(8);
    _heap_start = .;
    . = ORIGIN(RAM) + LENGTH(RAM) - 4K; /* Leave 4K for stack at the end of RAM */
    . = ALIGN(8);
    _heap_end = .;
  } > RAM

  .stack (NOLOAD) :
  {
    . = ALIGN(8);
    _sstack = .;
    . = . + 4K;
    . = ALIGN(8);
    _estack = .;
  } > RAM
}
