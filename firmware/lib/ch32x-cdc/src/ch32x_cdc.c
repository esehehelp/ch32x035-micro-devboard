/*
 * ch32x_cdc.c — USB CDC-ACM library for CH32X035.
 * SPDX-License-Identifier: MIT
 *
 * Placeholder — full implementation in Phase 2.
 */

#include "ch32x_regs.h"
#include "ch32x_cdc.h"

void ch32x_cdc_reboot_to_bootrom(void) {
    __disable_irq();
    USBFSD->BASE_CTRL &= ~USBFS_UC_DEV_PU_EN;
    for (volatile int i = 0; i < 100000; i++) __asm volatile("");
    RCC->RSTSCKR |= RCC_RSTSCKR_RMVF;
    FLASH->BOOT_MODEKEYR = FLASH_KEY1;
    FLASH->BOOT_MODEKEYR = FLASH_KEY2;
    FLASH->STATR |= FLASH_STATR_BOOT_MODE;
    PFIC->CFGR = PFIC_KEY3 | PFIC_SYSRST;
    for (;;) __asm volatile("nop");
}
