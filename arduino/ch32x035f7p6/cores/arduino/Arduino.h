/*
 * Arduino.h — Minimal Arduino API for CH32X035F7P6.
 * SPDX-License-Identifier: MIT
 */
#ifndef Arduino_h
#define Arduino_h

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "ch32x_regs.h"

/* Arduino constants */
#define HIGH    1
#define LOW     0
#define INPUT           0
#define OUTPUT          1
#define INPUT_PULLUP    2
#define INPUT_PULLDOWN  3

#define PI          3.1415926535897932384626433832795
#define HALF_PI     1.5707963267948966192313216916398
#define TWO_PI      6.283185307179586476925286766559
#define DEG_TO_RAD  0.017453292519943295769236907684886
#define RAD_TO_DEG  57.295779513082320876798154814105

#define min(a,b)    ((a)<(b)?(a):(b))
#define max(a,b)    ((a)>(b)?(a):(b))
#define abs(x)      ((x)>0?(x):-(x))
#define constrain(amt,lo,hi) ((amt)<(lo)?(lo):((amt)>(hi)?(hi):(amt)))
#define map(x,in_min,in_max,out_min,out_max) \
    (((x)-(in_min))*((out_max)-(out_min))/((in_max)-(in_min))+(out_min))

#define lowByte(w)   ((uint8_t)((w) & 0xFF))
#define highByte(w)  ((uint8_t)((w) >> 8))
#define bitRead(v,b)       (((v) >> (b)) & 0x01)
#define bitSet(v,b)        ((v) |= (1UL << (b)))
#define bitClear(v,b)      ((v) &= ~(1UL << (b)))
#define bitWrite(v,b,d)    ((d) ? bitSet(v,b) : bitClear(v,b))
#define bit(b)             (1UL << (b))

typedef bool     boolean;
typedef uint8_t  byte;
typedef uint16_t word;

/* Pin mapping (variant provides digital_pin_map[]) */
typedef struct {
    GPIO_TypeDef *port;
    uint8_t       bit;
} pin_info_t;

extern const pin_info_t digital_pin_map[];

/* Digital I/O */
void pinMode(uint8_t pin, uint8_t mode);
void digitalWrite(uint8_t pin, uint8_t val);
int  digitalRead(uint8_t pin);

/* Timing */
unsigned long millis(void);
unsigned long micros(void);
void delay(unsigned long ms);
void delayMicroseconds(unsigned int us);

/* User entry points */
void setup(void);
void loop(void);

#ifdef __cplusplus
}

/* C++ headers */
#include "WString.h"
#include "Print.h"
#include "Stream.h"
#include "USBSerial.h"

#endif

#include "pins_arduino.h"

#endif /* Arduino_h */
