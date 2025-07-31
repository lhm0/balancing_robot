#define DEBUG  // ← Debug-Ausgabe aktivieren; auskommentieren, um sie zu deaktivieren

#include <Arduino.h>
#include "IMU.h"
#include "Motor.h"
#include "DualMotor.h"
#include "Controller.h"
#include "Balancer.h"
#include "WebInterface.h"
#include "Secrets.h"

// 🔧 Debug-Makros
#ifdef DEBUG
  #define DEBUG_PRINT(x)    Serial.print(x)
  #define DEBUG_PRINTLN(x)  Serial.println(x)
  #define DEBUG_PRINTF(...) Serial.printf(__VA_ARGS__)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINTF(...)
#endif

// 🧠 Interrupt-Flag global definieren
volatile bool mpuInterrupt = false;

// 🧠 Interrupt-Routine
void IRAM_ATTR dmpDataReady() {
  mpuInterrupt = true;
}

// Pin-Konfiguration
constexpr int STEP1_PIN      = 26;
constexpr int FEEDBACK1_PIN  = 17;
constexpr int DIR1_PIN       = 25;
constexpr int STEP2_PIN      = 27;
constexpr int FEEDBACK2_PIN  = 16;
constexpr int DIR2_PIN       = 33;
constexpr int ENABLE_PIN     = 14;
constexpr int MPU_INT_PIN    = 5;

// Komponenteninstanzen
Motor leftMotor(STEP1_PIN, DIR1_PIN, ENABLE_PIN, true,  0, FEEDBACK1_PIN);  // PWM Channel 0
Motor rightMotor(STEP2_PIN, DIR2_PIN, ENABLE_PIN, false, 1, FEEDBACK2_PIN); // PWM Channel 1

DualMotor motors(leftMotor, rightMotor);
IMU imu;
Controller controller(900.0, 50.0, 100.0); // Startwerte: Kp, Ki, Kd
Balancer balancer(imu, motors, controller);
WebInterface web;

void setup() {
  Serial.begin(115200);
  delay(1000);

  motors.resetPositions();  // Position beim Start nullen
  imu.setAngleOffset(4.3);

  pinMode(MPU_INT_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(MPU_INT_PIN), dmpDataReady, RISING);

  balancer.begin();

  leftMotor.setDirection(true);
  leftMotor.setSpeed(500);  // Teststart: 500 Schritte/s

  web.connectToWiFi(WIFI_SSID, WIFI_PASS);
  web.setParameterReference(controller.getKpPointer(), controller.getKiPointer(), controller.getKdPointer());
  web.setIMUReference(&imu);
  web.setMotorReference(&motors);
  web.setControllerReference(&controller);
  web.begin();

  DEBUG_PRINTLN("Setup abgeschlossen.");
}

unsigned long lastUpdate = 0;
unsigned long lastStatus = 0;

void loop() {
  unsigned long now = millis();

  if (now - lastUpdate >= 1) {  // 100 Hz Update-Zyklus
    lastUpdate = now;
    balancer.update();
  }

  if (now - lastStatus >= 500) {
    lastStatus = now;

    float angle = imu.getAngleX();
    float kp = *controller.getKpPointer();
    float ki = *controller.getKiPointer();
    float kd = *controller.getKdPointer();
    float offset = imu.getAngleOffset();

    DEBUG_PRINTF("Winkel X: %.2f | Kp: %.2f | Ki: %.2f | Kd: %.2f | offset: %.2f\n",
                 angle, kp, ki, kd, offset);
  }
}