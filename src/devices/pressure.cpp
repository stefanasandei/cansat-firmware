//
// Created by Stefan on 17/5/2024.
//

#include "devices/pressure.hpp"
#include "devices/bmp085.hpp"

#include <avr/io.h>
#include <util/delay.h>

#include <string.h>

namespace CanSat {

    PressureSensor::PressureSensor() : i2c(4, 5) {
        init();
    }

    PressureSensor::~PressureSensor() {}

    void PressureSensor::init() {
        // Read calibration data
        i2c.start();
        i2c.send_byte(BMP085_ADDRESS | 0);// Write mode
        i2c.send_byte(0xAA);              // Calibration data start address
        i2c.restart();
        i2c.send_byte(BMP085_ADDRESS | 1);// Read mode

        // Read calibration coefficients
        ac1 = (int16_t) (i2c.get_byte(0) << 8 | i2c.get_byte(0));
        ac2 = (int16_t) (i2c.get_byte(0) << 8 | i2c.get_byte(0));
        ac3 = (int16_t) (i2c.get_byte(0) << 8 | i2c.get_byte(0));
        ac4 = (uint16_t) (i2c.get_byte(0) << 8 | i2c.get_byte(0));
        ac5 = (uint16_t) (i2c.get_byte(0) << 8 | i2c.get_byte(0));
        ac6 = (uint16_t) (i2c.get_byte(0) << 8 | i2c.get_byte(1));

        b1 = (int16_t) (i2c.get_byte(0) << 8 | i2c.get_byte(0));
        b2 = (int16_t) (i2c.get_byte(0) << 8 | i2c.get_byte(1));
        mb = (int16_t) (i2c.get_byte(0) << 8 | i2c.get_byte(0));
        mc = (int16_t) (i2c.get_byte(0) << 8 | i2c.get_byte(0));
        md = (int16_t) (i2c.get_byte(0) << 8 | i2c.get_byte(1));

        i2c.stop();
    }

    void PressureSensor::get_pressure(int32_t &value) {
        uint32_t up = read_up();
        int32_t ut = read_ut();
        int32_t b5 = compute_b5(ut);

        value = compute_pressure(up, b5) / 100.0f;
        // Return pressure in hPa
    }

    uint32_t PressureSensor::read_ut() {
        // Send temperature measurement command
        i2c.start();
        i2c.send_byte(BMP085_ADDRESS | 0);  // Write mode
        i2c.send_byte(BMP085_CTRL_MEAS_REG);// Control register address
        i2c.send_byte(BMP085_TEMP_CMD);     // Temperature command
        i2c.stop();

        // Wait for measurement to complete (4.5 ms for temperature)
        _delay_ms(5);

        // Read temperature data
        i2c.start();
        i2c.send_byte(BMP085_ADDRESS | 0);// Write mode
        i2c.send_byte(0xF6);              // Temperature data register
        i2c.restart();
        i2c.send_byte(BMP085_ADDRESS | 1);// Read mode

        uint32_t ut = (uint32_t) i2c.get_byte(0) << 8 | i2c.get_byte(0);
        i2c.stop();

        return ut;
    }

    uint32_t PressureSensor::read_up() {
        // Send pressure measurement command
        i2c.start();
        i2c.send_byte(BMP085_ADDRESS | 0);  // Write mode
        i2c.send_byte(BMP085_CTRL_MEAS_REG);// Control register address
        i2c.send_byte(BMP085_PRESSURE_CMD); // Pressure command
        i2c.stop();

        // Wait for measurement to complete
        _delay_ms(28);

        // Read pressure data
        i2c.start();
        i2c.send_byte(BMP085_ADDRESS | 0);// Write mode
        i2c.send_byte(0xF6);              // Pressure data register
        i2c.restart();
        i2c.send_byte(BMP085_ADDRESS | 1);// Read mode

        uint32_t msb = (uint32_t) i2c.get_byte(0) << 16;
        uint32_t lsb = (uint32_t) i2c.get_byte(0) << 8;
        uint32_t xlsb = (uint32_t) i2c.get_byte(0);
        i2c.stop();

        return (msb | lsb | xlsb) >> (8 - 3);// Correct shift based on oversampling
    }

    int32_t PressureSensor::compute_b5(int32_t ut) {
        int32_t x1 = ((ut - (int32_t) ac6) * (int32_t) ac5) >> 15;
        int32_t x2 = ((int32_t) mc << 11) / (x1 + (int32_t) md);
        return x1 + x2;
    }

    int32_t PressureSensor::compute_pressure(uint32_t up, int32_t b5) {
        int32_t b6 = b5 - 4000;
        int32_t x1 = (b2 * (b6 * b6 >> 12)) >> 11;
        int32_t x2 = (ac2 * b6) >> 11;
        int32_t x3 = x1 + x2;
        int32_t b3 = (((ac1 * 4 + x3) << 3) + 2) / 4;// Correct shift based on oversampling
        x1 = (ac3 * b6) >> 13;
        x2 = (b1 * (b6 * b6 >> 12)) >> 16;
        x3 = ((x1 + x2) + 2) >> 2;
        uint32_t b4 = (ac4 * (uint32_t) (x3 + 32768)) >> 15;
        uint32_t b7 = ((uint32_t) up - b3) * (50000 >> 3);// Correct shift based on oversampling
        int32_t p;
        if (b7 < 0x80000000)
            p = (b7 * 2) / b4;
        else
            p = (b7 / b4) * 2;
        x1 = (p >> 8) * (p >> 8);
        x1 = (x1 * 3038) >> 16;
        x2 = (-7357 * p) >> 16;
        p += (x1 + x2 + 3791) >> 4;
        return p;
    }

}// namespace CanSat
