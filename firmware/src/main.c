/*
 * Sample app: PC3 blink + USB CDC serial echo.
 * SPDX-License-Identifier: MIT
 */

#include "ch32x_regs.h"
#include "ch32x_cdc.h"

void NMI_Handler(void)       __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void NMI_Handler(void)       {}
void HardFault_Handler(void) { ch32x_cdc_reboot_to_bootrom(); }

/* SysTick (QingKe V4C, 64-bit, HCLK/8) */
#define STK_CTLR  (*(volatile uint32_t *)0xE000F000)
#define STK_CNTL  (*(volatile uint32_t *)0xE000F008)
#define STK_CNTH  (*(volatile uint32_t *)0xE000F00C)

static uint64_t stk_read(void) {
    uint32_t hi, lo, hi2;
    do { hi = STK_CNTH; lo = STK_CNTL; hi2 = STK_CNTH; } while (hi != hi2);
    return ((uint64_t)hi << 32) | lo;
}

/* HCLK/8 ticks per millisecond: 48MHz/8 = 6MHz → 6000 ticks/ms */
#define TICKS_PER_MS  6000ULL
#define BLINK_MS      500

int main(void) {
    ch32x_cdc_init(NULL);

    /* SysTick: enable, HCLK/8, no interrupt */
    STK_CTLR = 0;
    STK_CTLR = (1 << 4);
    STK_CTLR = 0x01;

    /* PC3 push-pull output (cdc_init already enabled GPIOC clock) */
    GPIOC->CFGLR = (GPIOC->CFGLR & ~(0xFu << 12)) | (0x3u << 12);

    uint64_t next_toggle = stk_read() + BLINK_MS * TICKS_PER_MS;
    for (;;) {
        uint8_t buf[64];
        size_t n = ch32x_cdc_read_buf(buf, sizeof(buf));
        if (n > 0)
            ch32x_cdc_write(buf, n);
        ch32x_cdc_poll();
        IWDG->CTLR = 0xAAAA;
        if (stk_read() >= next_toggle) {
            GPIOC->OUTDR ^= (1 << 3);
            next_toggle += BLINK_MS * TICKS_PER_MS;
        }
    }
}
