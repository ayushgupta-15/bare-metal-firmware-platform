  .syntax unified
  .cpu cortex-m4
  .thumb

  .global Reset_Handler
  .type Reset_Handler, %function

Reset_Handler:
  /* Initialize the stack pointer */
  ldr sp, =_estack

  /* Copy .data section from flash to RAM */
  ldr r0, =_sidata  /* Source */
  ldr r1, =_sdata   /* Destination start */
  ldr r2, =_edata   /* Destination end */
copy_data_loop:
  cmp r1, r2
  bge zero_bss
  ldr r3, [r0], #4
  str r3, [r1], #4
  b copy_data_loop

zero_bss:
  /* Zero .bss section */
  ldr r0, =_sbss
  ldr r1, =_ebss
  mov r2, #0
zero_bss_loop:
  cmp r0, r1
  bge call_system_init
  str r2, [r0], #4
  b zero_bss_loop

call_system_init:
  bl SystemInit

call_main:
  bl main

  /* If main returns, infinite loop */
infinite_loop:
  b infinite_loop
  .size Reset_Handler, .-Reset_Handler
