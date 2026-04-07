/*
 * pins_arduino.h — Pin mapping for CH32X035F7P6 Micro Devboard.
 *
 * CH32X035F7P6 (TSSOP20) available GPIO:
 *   PA0-PA7, PB1, PB12, PC1, PC3, PC18(SWDIO), PC19(SWCLK)
 *   PC14/15/16/17 reserved for USB CDC (CC1, CC2, D-, D+)
 *
 * SPDX-License-Identifier: MIT
 */
#ifndef pins_arduino_h
#define pins_arduino_h

#define NUM_DIGITAL_PINS    14
#define NUM_ANALOG_INPUTS   8

/* D11 = PC3 = devboard LED */
#define LED_BUILTIN         11

/*
 * Arduino pin  ->  GPIO
 *   D0  = PA0       (A0)
 *   D1  = PA1       (A1)
 *   D2  = PA2       (A2)
 *   D3  = PA3       (A3)
 *   D4  = PA4       (A4)
 *   D5  = PA5       (A5)
 *   D6  = PA6       (A6)
 *   D7  = PA7       (A7)
 *   D8  = PB1
 *   D9  = PB12
 *   D10 = PC1
 *   D11 = PC3       (LED)
 *   D12 = PC18      (SWDIO — usable as GPIO)
 *   D13 = PC19      (SWCLK — usable as GPIO)
 */

/* Analog pin aliases */
#define A0  0
#define A1  1
#define A2  2
#define A3  3
#define A4  4
#define A5  5
#define A6  6
#define A7  7

#endif /* pins_arduino_h */
