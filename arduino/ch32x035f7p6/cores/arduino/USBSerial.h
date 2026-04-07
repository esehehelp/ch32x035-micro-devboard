/*
 * USBSerial.h — USB CDC Serial class for CH32X035F7P6.
 * Wraps ch32x_cdc library as Arduino Stream.
 * SPDX-License-Identifier: MIT
 */
#ifndef USBSerial_h
#define USBSerial_h

#include "Stream.h"

extern "C" {
#include "ch32x_cdc.h"
}

class USBSerial : public Stream {
public:
    void begin(unsigned long baud = 0) { (void)baud; }
    void begin(unsigned long baud, uint8_t config) { (void)baud; (void)config; }
    void end() {}

    virtual int available(void) { return ch32x_cdc_available(); }
    virtual int peek(void)      { return ch32x_cdc_peek(); }
    virtual int read(void)      { return ch32x_cdc_read(); }
    virtual void flush(void)    { ch32x_cdc_flush(); }

    virtual size_t write(uint8_t c) {
        return ch32x_cdc_write(&c, 1);
    }
    virtual size_t write(const uint8_t *buf, size_t len) {
        return ch32x_cdc_write(buf, len);
    }

    operator bool() { return true; }

    using Print::write;
};

extern USBSerial Serial;

#endif /* USBSerial_h */
