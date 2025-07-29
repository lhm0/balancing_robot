#include "IMU.h"
#include <MPU6050_light.h>
#include <Wire.h>

MPU6050 mpu(Wire);

bool IMU::begin() {
    Wire.begin(21, 22); // SDA, SCL
    byte status = mpu.begin();
    if (status != 0) return false;
    mpu.calcGyroOffsets();
    lastUpdate = millis();
    return true;
}

void IMU::update() {
    mpu.update();
    float accAngle = mpu.getAngleX();
    float gyroRate = mpu.getGyroX();
    unsigned long now = millis();
    float dt = (now - lastUpdate) / 1000.0;
    lastUpdate = now;

    angleX = alpha * (angleX + gyroRate * dt) + (1 - alpha) * accAngle;
}

float IMU::getAngleX() const {
    return angleX;
}

float IMU::getGyroRateX() const {
    return mpu.getGyroX();
}
