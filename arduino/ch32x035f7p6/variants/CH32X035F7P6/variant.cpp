/*
 * variant.cpp — Pin mapping table for CH32X035F7P6 Micro Devboard.
 * SPDX-License-Identifier: MIT
 */

#include "Arduino.h"

extern "C" {

const pin_info_t digital_pin_map[NUM_DIGITAL_PINS] = {
    /* D0  */ { GPIOA,  0 },   /* PA0  (A0) */
    /* D1  */ { GPIOA,  1 },   /* PA1  (A1) */
    /* D2  */ { GPIOA,  2 },   /* PA2  (A2) */
    /* D3  */ { GPIOA,  3 },   /* PA3  (A3) */
    /* D4  */ { GPIOA,  4 },   /* PA4  (A4) */
    /* D5  */ { GPIOA,  5 },   /* PA5  (A5) */
    /* D6  */ { GPIOA,  6 },   /* PA6  (A6) */
    /* D7  */ { GPIOA,  7 },   /* PA7  (A7) */
    /* D8  */ { GPIOB,  1 },   /* PB1  */
    /* D9  */ { GPIOB, 12 },   /* PB12 */
    /* D10 */ { GPIOC,  1 },   /* PC1  */
    /* D11 */ { GPIOC,  3 },   /* PC3  (LED) */
    /* D12 */ { GPIOC, 18 },   /* PC18 (SWDIO) */
    /* D13 */ { GPIOC, 19 },   /* PC19 (SWCLK) */
};

} /* extern "C" */
