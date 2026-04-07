/*
 * wiring.c — millis/micros/delay for CH32X035F7P6.
 * Uses SysTick counter (64-bit, runs at HCLK/8).
 * SPDX-License-Identifier: MIT
 */

#include "Arduino.h"

/* SysTick registers (QingKe V4C) */
#define STK_BASE    0xE000F000U
#define STK_CTLR    (*(volatile uint32_t *)(STK_BASE + 0x00))
#define STK_CNTL    (*(volatile uint32_t *)(STK_BASE + 0x08))
#define STK_CNTH    (*(volatile uint32_t *)(STK_BASE + 0x0C))

/* SysTick ticks per microsecond (HCLK/8 per WCH convention) */
#define TICKS_PER_US  (F_CPU / 8000000UL)

static uint8_t systick_inited = 0;

static void systick_init(void) {
    if (systick_inited) return;
    systick_inited = 1;
    STK_CTLR = 0;          /* stop */
    STK_CTLR = (1 << 4);   /* bit 4: reload counter to 0 */
    STK_CTLR = 0x01;       /* enable, HCLK/8, no interrupt */
}

static uint64_t systick_read(void) {
    uint32_t hi, lo, hi2;
    do {
        hi  = STK_CNTH;
        lo  = STK_CNTL;
        hi2 = STK_CNTH;
    } while (hi != hi2);
    return ((uint64_t)hi << 32) | lo;
}

unsigned long millis(void) {
    systick_init();
    return (unsigned long)(systick_read() / (TICKS_PER_US * 1000ULL));
}

unsigned long micros(void) {
    systick_init();
    return (unsigned long)(systick_read() / TICKS_PER_US);
}

void delay(unsigned long ms) {
    unsigned long start = millis();
    while (millis() - start < ms)
        __asm volatile("");
}

void delayMicroseconds(unsigned int us) {
    systick_init();
    uint64_t start = systick_read();
    uint64_t target = (uint64_t)us * TICKS_PER_US;
    while (systick_read() - start < target)
        __asm volatile("");
}
