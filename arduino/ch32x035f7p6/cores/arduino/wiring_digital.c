/*
 * wiring_digital.c — pinMode / digitalWrite / digitalRead for CH32X035.
 * SPDX-License-Identifier: MIT
 */

#include "Arduino.h"

void pinMode(uint8_t pin, uint8_t mode) {
    if (pin >= NUM_DIGITAL_PINS) return;

    GPIO_TypeDef *port = digital_pin_map[pin].port;
    uint8_t bit = digital_pin_map[pin].bit;

    /* Enable clock for port */
    if (port == GPIOA)      RCC->APB2PCENR |= RCC_APB2Periph_GPIOA;
    else if (port == GPIOB) RCC->APB2PCENR |= RCC_APB2Periph_GPIOB;
    else if (port == GPIOC) RCC->APB2PCENR |= RCC_APB2Periph_GPIOC;

    /* Select config register and shift */
    volatile uint32_t *cfgr;
    uint8_t shift;
    if (bit < 8) {
        cfgr = &port->CFGLR;
        shift = bit * 4;
    } else if (bit < 16) {
        cfgr = &port->CFGHR;
        shift = (bit - 8) * 4;
    } else {
        cfgr = &port->CFGXR;
        shift = (bit - 16) * 4;
    }

    /* CNF[1:0]:MODE[1:0] — 4 bits per pin
     *   INPUT:          CNF=01 MODE=00 (floating)     = 0x4
     *   OUTPUT:         CNF=00 MODE=11 (push-pull 50M) = 0x3
     *   INPUT_PULLUP:   CNF=10 MODE=00 (pull-up/down)  = 0x8, OUTDR bit=1
     *   INPUT_PULLDOWN: CNF=10 MODE=00 (pull-up/down)  = 0x8, OUTDR bit=0
     */
    uint32_t cfg;
    switch (mode) {
    case INPUT:
        cfg = 0x4;
        break;
    case OUTPUT:
        cfg = 0x3;
        break;
    case INPUT_PULLUP:
        cfg = 0x8;
        if (bit < 16)
            port->BSHR = (1u << bit);
        else
            port->BSXR = (1u << (bit - 16));
        break;
    case INPUT_PULLDOWN:
        cfg = 0x8;
        if (bit < 16)
            port->BCR = (1u << bit);
        else
            port->BSXR = (1u << ((bit - 16) + 16));
        break;
    default:
        cfg = 0x4;
        break;
    }

    *cfgr = (*cfgr & ~(0xFu << shift)) | (cfg << shift);
}

void digitalWrite(uint8_t pin, uint8_t val) {
    if (pin >= NUM_DIGITAL_PINS) return;

    GPIO_TypeDef *port = digital_pin_map[pin].port;
    uint8_t bit = digital_pin_map[pin].bit;

    if (bit < 16) {
        if (val)
            port->BSHR = (1u << bit);
        else
            port->BSHR = (1u << (bit + 16));
    } else {
        if (val)
            port->BSXR = (1u << (bit - 16));
        else
            port->BSXR = (1u << ((bit - 16) + 16));
    }
}

int digitalRead(uint8_t pin) {
    if (pin >= NUM_DIGITAL_PINS) return LOW;

    GPIO_TypeDef *port = digital_pin_map[pin].port;
    uint8_t bit = digital_pin_map[pin].bit;

    return (port->INDR & (1u << bit)) ? HIGH : LOW;
}
