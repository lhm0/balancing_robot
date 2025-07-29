#include <Arduino.h>
#include "IMU.h"
#include "Motor.h"
#include "DualMotor.h"
#include "Controller.h"
#include "Balancer.h"
#include "WebInterface.h"
#include "Secrets.h"

// Pin-Konfiguration
constexpr int STEP1_PIN = 26;
constexpr int DIR1_PIN  = 25;
constexpr int STEP2_PIN = 27;
constexpr int DIR2_PIN  = 33;
constexpr int ENABLE_PIN = 14;

// Komponenteninstanzen
Motor leftMotor(STEP1_PIN, DIR1_PIN, ENABLE_PIN);
Motor rightMotor(STEP2_PIN, DIR2_PIN, ENABLE_PIN);
DualMotor motors(leftMotor, rightMotor);
IMU imu;
Controller controller(18.0, 0.8); // Startwerte für Kp, Kd
Balancer balancer(imu, motors, controller);
WebInterface web;

// Balancer-Task (FreeRTOS)
void balancerTask(void* parameter) {
  const TickType_t cycleTime = pdMS_TO_TICKS(5); // 200 Hz
  TickType_t lastWakeTime = xTaskGetTickCount();

  while (true) {
    balancer.update();
    vTaskDelayUntil(&lastWakeTime, cycleTime);
  }
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  balancer.begin();

  // Task starten (Core 0, prio 1)
  xTaskCreatePinnedToCore(
    balancerTask,
    "BalancerTask",
    4096,
    nullptr,
    1,
    nullptr,
    0
  );

  web.connectToWiFi(WIFI_SSID, WIFI_PASS);
  web.setParameterReference(controller.getKpPointer(), controller.getKdPointer());
  web.begin();
}

void loop() {
    delay(1000);
    Serial.println("Main loop alive");
}
