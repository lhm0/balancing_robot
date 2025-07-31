#include "Controller.h"
#include <Arduino.h>

Controller::Controller(float kp, float ki, float kd)
    : Kp(kp), Ki(ki), Kd(kd) {}

float Controller::compute(float angle, float rate) {
    float error = -angle; // Sollwinkel = 0
    integral += error;
    integral = constrain(integral, -100.0f, 100.0f);
    float derivative = -rate;

    return Kp * error + Ki * integral + Kd * derivative;
}

void Controller::setTunings(float kp, float ki, float kd) {
    Kp = kp;
    Ki = ki;
    Kd = kd;
}

float* Controller::getKpPointer() { return &Kp; }
float* Controller::getKiPointer() { return &Ki; }
float* Controller::getKdPointer() { return &Kd; }