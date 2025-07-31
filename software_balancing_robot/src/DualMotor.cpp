#include "DualMotor.h"

DualMotor::DualMotor(Motor& left, Motor& right)
    : leftMotor(left), rightMotor(right) {}

void DualMotor::begin() {
    leftMotor.begin();
    rightMotor.begin();
    stop();
}

void DualMotor::setSpeed(float speed) {
    bool dir = speed >= 0;
    float absSpeed = fabs(speed);
    leftMotor.setDirection(dir);
    rightMotor.setDirection(dir);
    leftMotor.setSpeed(absSpeed);
    rightMotor.setSpeed(absSpeed);
}

void DualMotor::setDifferential(float leftSpeed, float rightSpeed) {
    leftMotor.setDirection(leftSpeed >= 0);
    rightMotor.setDirection(rightSpeed >= 0);
    leftMotor.setSpeed(fabs(leftSpeed));
    rightMotor.setSpeed(fabs(rightSpeed));
}

void DualMotor::stop() {
    leftMotor.setSpeed(0);
    rightMotor.setSpeed(0);
}

long DualMotor::getAveragePosition() const {
    long left = -leftMotor.getPosition();
    long right = rightMotor.getPosition();  // ← Vorzeichen umkehren
    return (left + right) / 2;
}

void DualMotor::resetPositions() {
    leftMotor.resetPosition();
    rightMotor.resetPosition();
}