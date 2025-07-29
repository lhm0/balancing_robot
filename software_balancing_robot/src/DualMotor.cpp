#include "DualMotor.h"

DualMotor::DualMotor(Motor& left, Motor& right)
    : leftMotor(left), rightMotor(right) {}

void DualMotor::begin() {
    leftMotor.begin();
    rightMotor.begin();
}

void DualMotor::drive(float speed) {
    bool dir = speed >= 0;
    int steps = abs(speed);
    leftMotor.setDirection(dir);
    rightMotor.setDirection(dir);
    for (int i = 0; i < steps; i++) {
        leftMotor.step();
        rightMotor.step();
        delayMicroseconds(800); // einfache Geschwindigkeit
    }
}

void DualMotor::driveDifferential(float leftSpeed, float rightSpeed) {
    leftMotor.setDirection(leftSpeed >= 0);
    rightMotor.setDirection(rightSpeed >= 0);
    int lSteps = abs(leftSpeed);
    int rSteps = abs(rightSpeed);
    int maxSteps = max(lSteps, rSteps);

    for (int i = 0; i < maxSteps; i++) {
        if (i < lSteps) leftMotor.step();
        if (i < rSteps) rightMotor.step();
        delayMicroseconds(800);
    }
}

void DualMotor::stop() {
    // optional: set speed = 0 oder disable motors
}
