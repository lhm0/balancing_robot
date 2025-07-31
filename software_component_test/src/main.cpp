#include <Arduino.h>
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

// MPU6050 Objekt
MPU6050 mpu;

// Interrupt Pin
#define INTERRUPT_PIN 5

bool dmpReady = false;
uint8_t mpuIntStatus;
uint8_t devStatus;
uint16_t packetSize;
uint16_t fifoCount;
uint8_t fifoBuffer[64];

// Orientierung
Quaternion q;
VectorFloat gravity;
float ypr[3];

// Interrupt-Flag
volatile bool mpuInterrupt = false;
void IRAM_ATTR dmpDataReady() {
  mpuInterrupt = true;
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Wire.begin(21, 22);  // SDA = 21, SCL = 22 (ESP32 DevKit)

  Serial.println("MPU6050 initialisieren...");
  mpu.initialize();

  // Verbindung prüfen
  Serial.println(mpu.testConnection() ? "MPU6050 verbunden" : "Verbindung fehlgeschlagen");

  // DMP starten
  devStatus = mpu.dmpInitialize();

  // Optional: Kalibrierung
  mpu.setXGyroOffset(220);
  mpu.setYGyroOffset(76);
  mpu.setZGyroOffset(-85);
  mpu.setZAccelOffset(1688);

  if (devStatus == 0) {
    Serial.println("DMP initialisiert");
    mpu.setDMPEnabled(true);

    // Interrupt einrichten
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();

    dmpReady = true;
    packetSize = mpu.dmpGetFIFOPacketSize();
  } else {
    Serial.print("Fehler beim Initialisieren des DMP: ");
    Serial.println(devStatus);
  }
}

void loop() {
  if (!dmpReady) return;

  // Auf Interrupt oder FIFO warten
  if (!mpuInterrupt && fifoCount < packetSize) return;

  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();
  fifoCount = mpu.getFIFOCount();

  if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
    mpu.resetFIFO();
    Serial.println("FIFO overflow!");
    return;
  }

  if (mpuIntStatus & 0x02) {
    while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

    mpu.getFIFOBytes(fifoBuffer, packetSize);
    fifoCount -= packetSize;

    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);

    Serial.print("Yaw: ");
    Serial.print(ypr[0] * 180 / M_PI);
    Serial.print(" | Pitch: ");
    Serial.print(ypr[1] * 180 / M_PI);
    Serial.print(" | Roll: ");
    Serial.println(ypr[2] * 180 / M_PI);
  }
}