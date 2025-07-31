#ifndef IMU_H
#define IMU_H

#include <Wire.h>
#include "MPU6050_6Axis_MotionApps20.h"

class IMU {
public:
    IMU();
    void begin();
    void update();
    float getAngleX() const;  // Pitch
    float getGyroRateX() const;
    void setAngleOffset(float offset);  // setze auf Wert
    void adjustAngleOffset(float delta);  // ändere bestehenden Wert z. B. um ±0.1
    float getAngleOffset() const;

private:
 MPU6050 mpu;
    bool dmpReady = false;
    uint16_t packetSize = 0;
    uint8_t fifoBuffer[64];

    float ypr[3] = {0};  // yaw, pitch, roll
    float angleX = 0.0f;
    float gyroRateX = 0.0f;
    float angleOffset = 0.0f; 
};

#endif