#include <Wire.h>
#include <MPU6050_light.h>

MPU6050 mpu(Wire);

#define ENABLE_PIN   14

#define STEP1_PIN    26
#define DIR1_PIN     25

#define STEP2_PIN    27
#define DIR2_PIN     33

// Schritt-Funktion: führt Schritte in gewünschter Richtung aus
void stepMotor(int stepPin, int dirPin, bool direction, int steps, int delay_us) {
  digitalWrite(dirPin, direction);
  delayMicroseconds(50);  // WICHTIG
  for (int i = 0; i < steps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(delay_us);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(delay_us);
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(ENABLE_PIN, OUTPUT);
  digitalWrite(ENABLE_PIN, LOW); // Aktivieren (LOW = enable)

  pinMode(STEP1_PIN, OUTPUT);
  pinMode(DIR1_PIN, OUTPUT);
  pinMode(STEP2_PIN, OUTPUT);
  pinMode(DIR2_PIN, OUTPUT);

  Serial.println("Starte endlosen Motor-Test...");
}

void loop() {
  Serial.println("Motor 1 vorwärts");
  stepMotor(STEP1_PIN, DIR1_PIN, true, 200, 1500);
  Serial.println("Motor 1 rückwärts");
  stepMotor(STEP1_PIN, DIR1_PIN, false, 200, 1500);

  delay(1000); // Pause

  Serial.println("Motor 2 vorwärts");
  stepMotor(STEP2_PIN, DIR2_PIN, true, 200, 1500);
  Serial.println("Motor 2 rückwärts");
  stepMotor(STEP2_PIN, DIR2_PIN, false, 200, 1500);

  delay(1000); // Pause
}