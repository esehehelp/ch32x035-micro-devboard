/*
 * main.cpp — Arduino entry point for CH32X035F7P6.
 * CDC is unconditionally initialized for 1200bps upload support.
 * SPDX-License-Identifier: MIT
 */

#include "Arduino.h"

extern "C" {
#include "ch32x_cdc.h"
}

int main(void) {
    /* Clock + USB CDC (always active for upload support) */
    ch32x_cdc_init(NULL);

    setup();
    for (;;) {
        loop();
        ch32x_cdc_poll();
    }

    return 0;
}
