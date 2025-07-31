#include "IMU.h"

extern volatile bool mpuInterrupt;

IMU::IMU() {}

void IMU::begin() {
    Wire.begin();

    mpu.initialize();

    if (!mpu.testConnection()) {
        Serial.println("IMU: Verbindung zum MPU6050 fehlgeschlagen!");
        return;
    }

    Serial.println("IMU: DMP initialisieren...");
    if (mpu.dmpInitialize() != 0) {
        Serial.println("IMU: DMP-Initialisierung fehlgeschlagen!");
        return;
    }

    // Optional: Offsets setzen (angepasst auf dein Modul)
    mpu.setXGyroOffset(220);
    mpu.setYGyroOffset(76);
    mpu.setZGyroOffset(-85);
    mpu.setZAccelOffset(1788);

    mpu.resetFIFO();  // <<< WICHTIG: FIFO vor Start leeren!
    mpu.setDMPEnabled(true);
    dmpReady = true;
    packetSize = mpu.dmpGetFIFOPacketSize();

    Serial.println("IMU: DMP init erfolgreich");
}

void IMU::update() {
    if (!dmpReady) return;

    // Warten auf neuen Interrupt und ausreichende FIFO-Daten
    if (!mpuInterrupt && mpu.getFIFOCount() < packetSize) return;

    mpuInterrupt = false;
    uint8_t mpuIntStatus = mpu.getIntStatus();
    uint16_t fifoCount = mpu.getFIFOCount();

    // Überlaufbehandlung
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        mpu.resetFIFO();
        Serial.println("IMU: FIFO overflow!");
        return;
    }

    // Daten verfügbar?
   if (mpuIntStatus & 0x02) {
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        mpu.getFIFOBytes(fifoBuffer, packetSize);
        fifoCount -= packetSize;

        Quaternion q;
        VectorFloat gravity;

        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

        // Speichere Pitch (ypr[2]) und Gyro-Wert
        angleX = ypr[2] * 180.0f / M_PI;

        // Gyro-Daten auslesen (Winkelgeschwindigkeit)
        int16_t gx, gy, gz;
        mpu.getRotation(&gx, &gy, &gz);
        gyroRateX = static_cast<float>(gy) / 131.0f;    
    }
}

float IMU::getAngleX() const {
    return angleX - angleOffset;
}

void IMU::setAngleOffset(float offset) {
    angleOffset = offset;
}

void IMU::adjustAngleOffset(float delta) {
    angleOffset += delta;
}

float IMU::getAngleOffset() const {
    return angleOffset;
}

float IMU::getGyroRateX() const {
    return gyroRateX;
}