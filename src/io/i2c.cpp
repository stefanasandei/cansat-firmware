//
// Created by Stefan on 4/4/2024.
//

#include "io/i2c.hpp"

namespace CanSat {

    volatile uint8_t i2c_frame_error = 0;

    I2C::I2C() {
        I2C_DDR &= ~_BV(SDA);
        I2C_DDR &= ~_BV(SCL);

        I2C_PORT &= ~_BV(SDA);
        I2C_PORT &= ~_BV(SCL);

        stop();
    }

    I2C::~I2C() {}

    void I2C::start() const {
        I2C_DDR |= _BV(SDA);
        I2C_DELAY
        I2C_DDR |= _BV(SCL);
        I2C_DELAY
    }

    void I2C::restart() const {
        I2C_DDR &= ~_BV(SDA);
        I2C_DELAY
        I2C_DDR &= ~_BV(SCL);
        I2C_DELAY

        I2C_DDR |= _BV(SDA);
        I2C_DELAY
        I2C_DDR |= _BV(SCL);
        I2C_DELAY
    }

    void I2C::stop() const {
        I2C_DDR |= _BV(SCL);
        I2C_DELAY
        I2C_DDR |= _BV(SDA);
        I2C_DELAY

        I2C_DDR &= ~_BV(SCL);
        I2C_DELAY
        I2C_DDR &= ~_BV(SDA);
        I2C_DELAY

        i2c_frame_error = 0;

        if ((I2C_PIN & _BV(SDA)) == 0) i2c_frame_error++;
        if ((I2C_PIN & _BV(SCL)) == 0) i2c_frame_error++;

        I2C_DELAY
        I2C_DELAY
        I2C_DELAY
        I2C_DELAY
    }

    uint8_t I2C::send_byte(uint8_t data) const {
        uint8_t i;
        uint8_t ack = 1;

        for (i = 0; i < 8; i++) {
            if ((data & 0x80) == 0x00) I2C_DDR |= _BV(SDA);
            else
                I2C_DDR &= ~_BV(SDA);

            I2C_DELAY
            I2C_DDR &= ~_BV(SCL);

            I2C_DELAY
            I2C_DDR |= _BV(SCL);

            data = data << 1;
        }

        I2C_DDR &= ~_BV(SDA);
        I2C_DELAY

        I2C_DDR &= ~_BV(SCL);
        I2C_DELAY

        if ((I2C_PIN & _BV(SDA)) == _BV(SDA)) ack = 1;
        else
            ack = 0;

        I2C_DDR |= _BV(SCL);

        return ack;
    }

    uint8_t I2C::get_byte(uint8_t last_byte) const {
        uint8_t i, res = 0;
        I2C_DDR &= ~_BV(SDA);

        for (i = 0; i < 8; i++) {
            res = res << 1;

            I2C_DDR &= ~_BV(SCL);
            I2C_DELAY

            if ((I2C_PIN & _BV(SDA)) == _BV(SDA)) res = res | 0x01;

            I2C_DDR |= _BV(SCL);
            I2C_DELAY
        }

        if (last_byte == 0)
            I2C_DDR |= _BV(SDA);
        else
            I2C_DDR &= ~_BV(SDA);

        I2C_DELAY
        I2C_DDR &= ~_BV(SCL);

        I2C_DELAY
        I2C_DDR |= _BV(SCL);

        I2C_DELAY
        I2C_DDR &= ~_BV(SDA);

        return res;
    }

}// namespace CanSat
