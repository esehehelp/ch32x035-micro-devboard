/*
 * Phase 1: Blink PC3 ~3s then reboot to BootROM.
 * SPDX-License-Identifier: MIT
 */

#include "ch32x_regs.h"
#include "ch32x_cdc.h"

void NMI_Handler(void)       __attribute__((interrupt("WCH-Interrupt-fast")));
void HardFault_Handler(void) __attribute__((interrupt("WCH-Interrupt-fast")));
void NMI_Handler(void)       {}
void HardFault_Handler(void) { for (;;) {} }

int main(void) {
    /* HSI on, 2 wait-states for 48 MHz */
    RCC->CTLR |= 1;
    FLASH->ACTLR = (FLASH->ACTLR & ~0x03) | 0x02;

    /* PC3: output push-pull 50 MHz (CNF=00 MODE=11 = 0x3) */
    RCC->APB2PCENR |= RCC_APB2Periph_GPIOC;
    GPIOC->CFGLR = (GPIOC->CFGLR & ~(0xFu << 12)) | (0x3u << 12);

    /* Blink PC3 ~3 seconds (6 toggles × ~500ms) */
    for (int n = 0; n < 6; n++) {
        GPIOC->OUTDR ^= GPIO_Pin_3;
        IWDG->CTLR = 0xAAAA;
        for (volatile uint32_t i = 0; i < 200000; i++)
            __asm volatile("");
    }

    /* Enter BootROM — wchisp should be able to connect after this */
    ch32x_cdc_reboot_to_bootrom();
}
