  .syntax unified
  .cpu cortex-m4
  .thumb

  .global g_pfnVectors
  .section .isr_vector,"a",%progbits

g_pfnVectors:
  .word _estack
  .word Reset_Handler
  .word NMI_Handler
  .word HardFault_Handler
  .word MemManage_Handler
  .word BusFault_Handler
  .word UsageFault_Handler
  .word 0 /* Reserved */
  .word 0 /* Reserved */
  .word 0 /* Reserved */
  .word 0 /* Reserved */
  .word SVC_Handler
  .word 0 /* DebugMon */
  .word 0 /* Reserved */
  .word PendSV_Handler
  .word SysTick_Handler

  /* External Interrupts */
  .space (28 * 4)
  .word TIM2_IRQHandler   /* 28: TIM2 */
  .space (8 * 4)          /* 29-36 */
  .word USART1_IRQHandler /* 37: USART1 */
  .word USART2_IRQHandler /* 38: USART2 */

  /* Provide weak aliases for the default handlers */
  .weak NMI_Handler
  .thumb_set NMI_Handler, Default_Handler

  .weak HardFault_Handler
  .thumb_set HardFault_Handler, Default_Handler

  .weak MemManage_Handler
  .thumb_set MemManage_Handler, Default_Handler

  .weak BusFault_Handler
  .thumb_set BusFault_Handler, Default_Handler

  .weak UsageFault_Handler
  .thumb_set UsageFault_Handler, Default_Handler

  .weak SVC_Handler
  .thumb_set SVC_Handler, Default_Handler

  .weak PendSV_Handler
  .thumb_set PendSV_Handler, Default_Handler

  .weak SysTick_Handler
  .thumb_set SysTick_Handler, Default_Handler

  .weak TIM2_IRQHandler
  .thumb_set TIM2_IRQHandler, Default_Handler

  .weak USART1_IRQHandler
  .thumb_set USART1_IRQHandler, Default_Handler

  .weak USART2_IRQHandler
  .thumb_set USART2_IRQHandler, Default_Handler

  .section .text.Default_Handler,"ax",%progbits
Default_Handler:
  b Default_Handler
