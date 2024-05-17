//
// Created by Stefan on 17/5/2024.
//

#ifndef PRESSURE_HPP
#define PRESSURE_HPP

#include "cansat.hpp"
#include "io/i2c.hpp"

#include <avr/eeprom.h>
#include <avr/io.h>

namespace CanSat {

    class PressureSensor {
    public:
        PressureSensor();
        ~PressureSensor();

        void get_pressure(int32_t &value);

    private:
        void init();

    private:
        I2C i2c;

        // Calibration coefficients
        int16_t ac1, ac2, ac3;
        uint16_t ac4, ac5, ac6;
        int16_t b1, b2;
        int16_t mb, mc, md;

        // Read uncompensated temperature and pressure
        uint32_t read_ut();
        uint32_t read_up();

        // Temperature and pressure calculations
        int32_t compute_b5(int32_t ut);
        int32_t compute_pressure(uint32_t up, int32_t b5);

        // BMP085 commands
        enum : uint8_t {
            BMP085_ADDRESS = 0xEE,
            BMP085_CTRL_MEAS_REG = 0xF4,
            BMP085_TEMP_CMD = 0x2E,
            BMP085_PRESSURE_CMD = 0x34
        };
    };

}// namespace CanSat

#endif//PRESSURE_HPP
