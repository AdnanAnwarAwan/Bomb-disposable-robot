/**
 * @file    imu.cpp
 * @brief   MPU-6050 tip-over detection for stability safety
 *
 * I2C address: 0x68 (AD0 low)
 * Tip-over threshold: |roll| > 45° or |pitch| > 40°
 *
 * @author  Adnan Anwar Awan
 */

#include "imu.h"
#include <Wire.h>
#include <Arduino.h>

#define MPU6050_ADDR        0x68
#define MPU6050_PWR_MGMT_1  0x6B
#define MPU6050_ACCEL_XOUT  0x3B
#define ACCEL_SCALE         16384.0f   // ±2g range → LSB/g

#define TIPOVER_ROLL_WARN   35.0f
#define TIPOVER_ROLL_STOP   45.0f
#define TIPOVER_PITCH_WARN  30.0f
#define TIPOVER_PITCH_STOP  40.0f

static float roll_deg  = 0.0f;
static float pitch_deg = 0.0f;

void IMU_Init() {
    Wire.begin();
    // Wake MPU-6050 from sleep
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(MPU6050_PWR_MGMT_1);
    Wire.write(0x00);    // Clear sleep bit
    Wire.endTransmission();
    delay(100);
    Serial.println(F("IMU: MPU-6050 ready"));
}

static void readAccel(float &ax, float &ay, float &az) {
    Wire.beginTransmission(MPU6050_ADDR);
    Wire.write(MPU6050_ACCEL_XOUT);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU6050_ADDR, 6, true);

    int16_t raw_ax = (Wire.read() << 8) | Wire.read();
    int16_t raw_ay = (Wire.read() << 8) | Wire.read();
    int16_t raw_az = (Wire.read() << 8) | Wire.read();

    ax = raw_ax / ACCEL_SCALE;
    ay = raw_ay / ACCEL_SCALE;
    az = raw_az / ACCEL_SCALE;
}

void IMU_Update() {
    float ax, ay, az;
    readAccel(ax, ay, az);

    // Calculate roll and pitch from accelerometer
    roll_deg  = atan2f(ay, az) * 180.0f / PI;
    pitch_deg = atan2f(-ax, sqrtf(ay*ay + az*az)) * 180.0f / PI;
}

bool IMU_TipOverDetected() {
    IMU_Update();
    return (fabsf(roll_deg)  > TIPOVER_ROLL_STOP ||
            fabsf(pitch_deg) > TIPOVER_PITCH_STOP);
}

float IMU_GetRoll()  { return roll_deg; }
float IMU_GetPitch() { return pitch_deg; }
