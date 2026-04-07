/*
 * CH32X035 startup — full vector table with weak ISR symbols.
 * SPDX-License-Identifier: MIT
 */

    .section .init,"ax",@progbits
    .global _start
    .align  1
_start:
    j   handle_reset

    /* ── Vector table (46 entries: reset + 45 interrupts) ────── */
    .section .vector,"ax",@progbits
    .align  1
    .global _vector_base
_vector_base:
    .option norvc
    .word   _start              /*  0  Reset                    */
    .word   0                   /*  1  Reserved                 */
    .word   NMI_Handler         /*  2  NMI                      */
    .word   HardFault_Handler   /*  3  HardFault                */
    .word   0                   /*  4  Reserved                 */
    .word   0                   /*  5  Ecall-M                  */
    .word   0                   /*  6  Reserved                 */
    .word   0                   /*  7  Reserved                 */
    .word   0                   /*  8  Ecall-U                  */
    .word   0                   /*  9  Break                    */
    .word   0                   /* 10  Reserved                 */
    .word   0                   /* 11  Reserved                 */
    .word   SysTick_Handler     /* 12  SysTick                  */
    .word   0                   /* 13  Reserved                 */
    .word   0                   /* 14  SW                       */
    .word   0                   /* 15  Reserved                 */
    /* External interrupts 16..44 (29 entries) */
    .word   WWDG_IRQHandler           /* 16 */
    .word   PVD_IRQHandler            /* 17 */
    .word   FLASH_IRQHandler          /* 18 */
    .word   0                         /* 19 */
    .word   EXTI7_0_IRQHandler        /* 20 */
    .word   AWU_IRQHandler            /* 21 */
    .word   DMA1_Channel1_IRQHandler  /* 22 */
    .word   DMA1_Channel2_IRQHandler  /* 23 */
    .word   DMA1_Channel3_IRQHandler  /* 24 */
    .word   DMA1_Channel4_IRQHandler  /* 25 */
    .word   DMA1_Channel5_IRQHandler  /* 26 */
    .word   DMA1_Channel6_IRQHandler  /* 27 */
    .word   DMA1_Channel7_IRQHandler  /* 28 */
    .word   ADC1_IRQHandler           /* 29 */
    .word   I2C1_EV_IRQHandler        /* 30 */
    .word   I2C1_ER_IRQHandler        /* 31 */
    .word   USART1_IRQHandler         /* 32 */
    .word   SPI1_IRQHandler           /* 33 */
    .word   TIM1_BRK_IRQHandler       /* 34 */
    .word   TIM1_UP_IRQHandler        /* 35 */
    .word   TIM1_TRG_COM_IRQHandler   /* 36 */
    .word   TIM1_CC_IRQHandler        /* 37 */
    .word   TIM2_UP_IRQHandler        /* 38 */
    .word   USART2_IRQHandler         /* 39 */
    .word   EXTI15_8_IRQHandler       /* 40 */
    .word   EXTI25_16_IRQHandler      /* 41 */
    .word   USART3_IRQHandler         /* 42 */
    .word   USART4_IRQHandler         /* 43 */
    .word   PIOC_IRQHandler           /* 44 */
    .word   USBFS_IRQHandler          /* 45 USBFS */
    .option rvc

    /* ── Weak default handlers ───────────────────────────────── */
    .weak NMI_Handler
    .weak HardFault_Handler
    .weak SysTick_Handler
    .weak WWDG_IRQHandler
    .weak PVD_IRQHandler
    .weak FLASH_IRQHandler
    .weak EXTI7_0_IRQHandler
    .weak AWU_IRQHandler
    .weak DMA1_Channel1_IRQHandler
    .weak DMA1_Channel2_IRQHandler
    .weak DMA1_Channel3_IRQHandler
    .weak DMA1_Channel4_IRQHandler
    .weak DMA1_Channel5_IRQHandler
    .weak DMA1_Channel6_IRQHandler
    .weak DMA1_Channel7_IRQHandler
    .weak ADC1_IRQHandler
    .weak I2C1_EV_IRQHandler
    .weak I2C1_ER_IRQHandler
    .weak USART1_IRQHandler
    .weak SPI1_IRQHandler
    .weak TIM1_BRK_IRQHandler
    .weak TIM1_UP_IRQHandler
    .weak TIM1_TRG_COM_IRQHandler
    .weak TIM1_CC_IRQHandler
    .weak TIM2_UP_IRQHandler
    .weak USART2_IRQHandler
    .weak EXTI15_8_IRQHandler
    .weak EXTI25_16_IRQHandler
    .weak USART3_IRQHandler
    .weak USART4_IRQHandler
    .weak PIOC_IRQHandler
    .weak USBFS_IRQHandler

    .section .text.default_handler,"ax",@progbits
NMI_Handler:
HardFault_Handler:
SysTick_Handler:
WWDG_IRQHandler:
PVD_IRQHandler:
FLASH_IRQHandler:
EXTI7_0_IRQHandler:
AWU_IRQHandler:
DMA1_Channel1_IRQHandler:
DMA1_Channel2_IRQHandler:
DMA1_Channel3_IRQHandler:
DMA1_Channel4_IRQHandler:
DMA1_Channel5_IRQHandler:
DMA1_Channel6_IRQHandler:
DMA1_Channel7_IRQHandler:
ADC1_IRQHandler:
I2C1_EV_IRQHandler:
I2C1_ER_IRQHandler:
USART1_IRQHandler:
SPI1_IRQHandler:
TIM1_BRK_IRQHandler:
TIM1_UP_IRQHandler:
TIM1_TRG_COM_IRQHandler:
TIM1_CC_IRQHandler:
TIM2_UP_IRQHandler:
USART2_IRQHandler:
EXTI15_8_IRQHandler:
EXTI25_16_IRQHandler:
USART3_IRQHandler:
USART4_IRQHandler:
PIOC_IRQHandler:
USBFS_IRQHandler:
1:  j   1b

    /* ── Reset handler ───────────────────────────────────────── */
    .section .text.handle_reset,"ax",@progbits
    .weak   handle_reset
    .align  1
handle_reset:
.option push
.option norelax
    la  gp, __global_pointer$
.option pop
    la  sp, _eusrstack

    /* Copy .data from flash to RAM */
    la  a0, _data_lma
    la  a1, _data_vma
    la  a2, _edata
    bgeu a1, a2, 2f
1:  lw  t0, (a0)
    sw  t0, (a1)
    addi a0, a0, 4
    addi a1, a1, 4
    bltu a1, a2, 1b
2:
    /* Clear .bss */
    la  a0, _sbss
    la  a1, _ebss
    bgeu a0, a1, 2f
1:  sw  zero, (a0)
    addi a0, a0, 4
    bltu a0, a1, 1b
2:
    /* Pipeline + instruction prediction */
    li  t0, 0x1f
    csrw 0xbc0, t0
    /* Interrupt nesting + hardware stack */
    li  t0, 0x3
    csrw 0x804, t0
    /* Vector table: absolute vectored mode (mode 3) */
    la  t0, _vector_base
    ori t0, t0, 3
    csrw mtvec, t0
    /* Global interrupt enable (MIE + MPIE) */
    li  t0, 0x88
    csrw mstatus, t0

    /* Jump to main */
    la  t0, main
    csrw mepc, t0
    mret
