#include "Motor.h"

Motor* Motor::instance0 = nullptr;
Motor* Motor::instance1 = nullptr;

Motor::Motor(int stepPin, int dirPin, int enablePin, bool reversed, int pwmChannel, int stepSensePin)
    : stepPin(stepPin), dirPin(dirPin), enablePin(enablePin),
      reversed(reversed), pwmChannel(pwmChannel), stepSensePin(stepSensePin) {}

void Motor::begin() {
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    pinMode(enablePin, OUTPUT);
    pinMode(stepSensePin, INPUT);  // nur zum Lesen

    disable();

    // LEDC Setup
    ledcSetup(pwmChannel, 1000, 8);  // Default-Freq, Duty = 8 Bit
    ledcAttachPin(stepPin, pwmChannel);

    // ISR Setup
    if (pwmChannel == 0) {
        instance0 = this;
        attachInterrupt(digitalPinToInterrupt(stepSensePin), onStepISR0, RISING);
    } else if (pwmChannel == 1) {
        instance1 = this;
        attachInterrupt(digitalPinToInterrupt(stepSensePin), onStepISR1, RISING);
    }
}

void Motor::setDirection(bool dir) {
    currentDirection = reversed ? !dir : dir;
    digitalWrite(dirPin, currentDirection);
}

void Motor::setSpeed(float stepsPerSecond) {
    if (stepsPerSecond <= 0) {
        ledcWriteTone(pwmChannel, 0);  // PWM aus
        return;
    }

    float freq = stepsPerSecond;
    freq = constrain(freq, 1, 10000);  // Sicherer Bereich
    ledcWriteTone(pwmChannel, freq);
    ledcWrite(pwmChannel, 128);  // 50% Duty
    enable();
}

void Motor::enable() {
    digitalWrite(enablePin, LOW);  // meist LOW = aktiv
}

void Motor::disable() {
    digitalWrite(enablePin, HIGH);
}

float Motor::getSpeed() const {
    return ledcReadFreq(pwmChannel);  // Frequenz in Hz
}

long Motor::getPosition() const {
    return position;
}

void Motor::resetPosition() {
    position = 0;
}

// ISR-Funktionen
void IRAM_ATTR Motor::onStepISR0() {
    if (instance0) instance0->handleStepISR();
}

void IRAM_ATTR Motor::onStepISR1() {
    if (instance1) instance1->handleStepISR();
}

// Zähl-Logik
void Motor::handleStepISR() {
    if (currentDirection)
        position++;
    else
        position--;
}