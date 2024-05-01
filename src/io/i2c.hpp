//
// Created by Stefan on 4/4/2024.
//

#ifndef I2C_HPP
#define I2C_HPP

#include "cansat.hpp"

#include <avr/io.h>
#include <util/delay.h>

#define SDA 4
#define SCL 5
#define I2C_PORT PORTC
#define I2C_PIN PINC
#define I2C_DDR DDRC
#define I2C_DELAY _delay_us(4);

namespace CanSat {

    class I2C {
    public:
        I2C();
        ~I2C();

        void start() const;
        void restart() const;
        void stop() const;

        unsigned char send_byte(unsigned char data) const;
        unsigned char get_byte(unsigned char last_byte) const;
    };

}// namespace CanSat

#endif//I2C_HPP