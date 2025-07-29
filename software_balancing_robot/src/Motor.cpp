#include "Motor.h"

Motor::Motor(int stepPin, int dirPin, int enablePin)
    : stepPin(stepPin), dirPin(dirPin), enablePin(enablePin) {}

void Motor::begin() {
    pinMode(stepPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    if (enablePin >= 0) {
        pinMode(enablePin, OUTPUT);
        digitalWrite(enablePin, LOW); // aktiv
    }
}

void Motor::setDirection(bool forward) {
    direction = forward;
    digitalWrite(dirPin, direction);
    delayMicroseconds(50);
}

void Motor::step() {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(2);
    digitalWrite(stepPin, LOW);
}

void Motor::enable() {
    if (enablePin >= 0) digitalWrite(enablePin, LOW);
}

void Motor::disable() {
    if (enablePin >= 0) digitalWrite(enablePin, HIGH);
}
