//
// Created by Stefan on 4/4/2024.
//

#include "devices/accelerometer.hpp"

#include <math.h>
#include <stdlib.h>

namespace CanSat {

    Accelerometer::Accelerometer() {
        m_AccelData = (int32_t *) malloc(3 * sizeof(int32_t));
        m_GyroData = (int32_t *) malloc(3 * sizeof(int32_t));

        m_AccelAngles = (float *) malloc(3 * sizeof(float));
        m_GyroAngles = (float *) malloc(3 * sizeof(float));

        m_Data = (float *) malloc(3 * sizeof(float));
        m_Data[0] = m_Data[1] = m_Data[2] = 0.0f;

        init();
    }

    Accelerometer::~Accelerometer() {
        free(m_AccelData);
        free(m_GyroData);

        free(m_AccelAngles);
        free(m_GyroAngles);

        free(m_Data);
    }

    void Accelerometer::get_rotation(SpatialData &data) {
        filtered_angles(m_Data);

        data.x = m_GyroAngles[0];
        data.y = m_GyroAngles[1];
        data.z = m_GyroAngles[2];
    }

    void Accelerometer::get_temp(int32_t &data) {
        int32_t value = get_reg(TEMP_OUT_H, 2);
        data = value / 340 + 31.53f;
    }

    void Accelerometer::get_accel() {
        m_AccelData[0] = get_reg(ACCEL_XOUT_H, 2);
        m_AccelData[1] = get_reg(ACCEL_YOUT_H, 2);
        m_AccelData[2] = get_reg(ACCEL_ZOUT_H, 2);
    }

    void Accelerometer::get_gyro() {
        m_GyroData[0] = get_reg(GYRO_XOUT_H, 2);
        m_GyroData[1] = get_reg(GYRO_YOUT_H, 2);
        m_GyroData[2] = get_reg(GYRO_ZOUT_H, 2);
    }

    float Accelerometer::accel_angle(float a, float b, float c) const {
        return 57.295f * atan(a / sqrt(pow(b, 2) + pow(c, 2)));
    }

    float Accelerometer::gryo_angle(float prev_data, int32_t data, int32_t delta) const {
        return prev_data + data * delta * 0.001f;
    }

    void Accelerometer::accel_angles() {
        get_accel();

        m_AccelAngles[0] = accel_angle(m_AccelData[0], m_AccelData[1], m_AccelData[2]);
        m_AccelAngles[1] = accel_angle(m_AccelData[1], m_AccelData[0], m_AccelData[2]);
        m_AccelAngles[2] = accel_angle(m_AccelData[2], m_AccelData[0], m_AccelData[1]);
    }

    void Accelerometer::gyro_angles(float *prev_data, int32_t delta) {
        get_gyro();

        m_GyroAngles[0] = gryo_angle(prev_data[0], m_GyroData[0], delta);
        m_GyroAngles[1] = gryo_angle(prev_data[1], m_GyroData[1], delta);
        m_GyroAngles[2] = gryo_angle(prev_data[2], m_GyroData[2], delta);
    }


    void Accelerometer::filtered_angles(float *prev_data) {
        accel_angles();
        gyro_angles(prev_data, 200);

        filter(prev_data);
    }

    void Accelerometer::filter(float *prev_data) {
        const float LAMBDA = 0.5f;

        m_Data[0] = LAMBDA * m_AccelAngles[0] + (1 - LAMBDA) * prev_data[0];
        m_Data[1] = LAMBDA * m_AccelAngles[1] + (1 - LAMBDA) * prev_data[1];
        m_Data[2] = LAMBDA * m_AccelAngles[2] + (1 - LAMBDA) * prev_data[2];
    }

    void Accelerometer::write_reg(uint8_t reg, uint8_t value) const {
        m_I2C.start();
        m_I2C.send_byte(MPU6050_W);
        m_I2C.send_byte(reg);
        m_I2C.send_byte(value);
        m_I2C.stop();
    }

    void Accelerometer::move_to_reg(uint8_t reg) const {
        m_I2C.start();
        m_I2C.send_byte(MPU6050_W);
        m_I2C.send_byte(reg);
        m_I2C.stop();
    }

    int32_t Accelerometer::get_reg(uint8_t reg, uint8_t len) const {
        move_to_reg(reg);

        m_I2C.start();
        m_I2C.send_byte(MPU6050_R);

        int32_t value = 0;
        for (uint8_t i = 0; i < len - 1; i++) {
            value += m_I2C.get_byte(0) << 8;
        }
        value += m_I2C.get_byte(1);

        m_I2C.stop();

        return value;
    }

    void Accelerometer::init() const {
        //Sets sample rate to 8000/1+7 = 1000Hz
        write_reg(SMPLRT_DIV, 0x07);

        //Disable FSync, 256Hz DLPF
        write_reg(CONFIG, 0x00);

        //Disable gyro self tests, scale of 2000 degrees/s
        write_reg(GYRO_CONFIG, 0b00011000);

        //Disable accel self tests, scale of +-2g, no DHPF
        write_reg(ACCEL_CONFIG, 0x00);

        //Freefall threshold of |0mg|
        write_reg(FF_THR, 0x00);

        //Freefall duration limit of 0
        write_reg(FF_DUR, 0x00);

        //Motion threshold of 0mg
        write_reg(MOT_THR, 0x00);

        //Motion duration of 0s
        write_reg(MOT_DUR, 0x00);

        //Zero motion threshold
        write_reg(ZRMOT_THR, 0x00);

        //Zero motion duration threshold
        write_reg(ZRMOT_DUR, 0x00);

        //Disable sensor output to FIFO buffer
        write_reg(FIFO_EN, 0x00);

        //AUX I2C setup
        //Sets AUX I2C to single master control, plus other config
        write_reg(I2C_MST_CTRL, 0x00);
        //Setup AUX I2C slaves
        write_reg(I2C_SLV0_ADDR, 0x00);
        write_reg(I2C_SLV0_REG, 0x00);
        write_reg(I2C_SLV0_CTRL, 0x00);
        write_reg(I2C_SLV1_ADDR, 0x00);
        write_reg(I2C_SLV1_REG, 0x00);
        write_reg(I2C_SLV1_CTRL, 0x00);
        write_reg(I2C_SLV2_ADDR, 0x00);
        write_reg(I2C_SLV2_REG, 0x00);
        write_reg(I2C_SLV2_CTRL, 0x00);
        write_reg(I2C_SLV3_ADDR, 0x00);
        write_reg(I2C_SLV3_REG, 0x00);
        write_reg(I2C_SLV3_CTRL, 0x00);
        write_reg(I2C_SLV4_ADDR, 0x00);
        write_reg(I2C_SLV4_REG, 0x00);
        write_reg(I2C_SLV4_DO, 0x00);
        write_reg(I2C_SLV4_CTRL, 0x00);
        write_reg(I2C_SLV4_DI, 0x00);
        //I2C_MST_STATUS //Read-only
        //Setup INT pin and AUX I2C pass through
        write_reg(INT_PIN_CFG, 0x00);
        //Enable data ready interrupt
        write_reg(INT_ENABLE, 0x00);

        //Slave out, dont care
        write_reg(I2C_SLV0_DO, 0x00);
        write_reg(I2C_SLV1_DO, 0x00);
        write_reg(I2C_SLV2_DO, 0x00);
        write_reg(I2C_SLV3_DO, 0x00);
        //More slave config
        write_reg(I2C_MST_DELAY_CTRL, 0x00);
        //Reset sensor signal paths
        write_reg(SIGNAL_PATH_RESET, 0x00);
        //Motion detection control
        write_reg(MOT_DETECT_CTRL, 0x00);
        //Disables FIFO, AUX I2C, FIFO and I2C reset bits to 0
        write_reg(USER_CTRL, 0x00);
        //Sets clock source to gyro reference w/ PLL
        write_reg(PWR_MGMT_1, 0b00000010);
        //Controls frequency of wakeups in accel low power mode plus the sensor standby modes
        write_reg(PWR_MGMT_2, 0x00);
    }

}// namespace CanSat
