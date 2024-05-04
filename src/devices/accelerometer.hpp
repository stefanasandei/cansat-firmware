//
// Created by Stefan on 4/4/2024.
//

#ifndef ACCELEROMETER_HPP
#define ACCELEROMETER_HPP

#include "devices/mpu6050.hpp"
#include "io/i2c.hpp"

#include <stdint.h>

namespace CanSat {

    struct SpatialData {
        int32_t x, y, z;
    };

    class Accelerometer {
    public:
        Accelerometer();
        ~Accelerometer();

        void get_rotation(SpatialData &data);

    private:
        void init() const;

        void get_accel();
        void get_gyro();

        float accel_angle(float a, float b, float c) const;
        float gryo_angle(float prev_data, int32_t data, int32_t delta) const;

        void accel_angles();
        void gyro_angles(float *prev_data, int32_t delta);

        void filtered_angles(float *prev_data);
        void filter(float *prev_data);

        void write_reg(uint8_t reg, uint8_t value) const;
        void move_to_reg(uint8_t reg) const;
        int32_t get_reg(uint8_t reg, uint8_t len) const;

    private:
        I2C m_I2C;

        int32_t *m_AccelData, *m_GyroData;
        float *m_AccelAngles, *m_GyroAngles;
        float *m_Data;
    };

}// namespace CanSat

#endif//ACCELEROMETER_HPP
