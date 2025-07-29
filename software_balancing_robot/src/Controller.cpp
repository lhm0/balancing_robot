#include "Controller.h"

Controller::Controller(float kp, float kd) : Kp(kp), Kd(kd) {}

float Controller::compute(float angle, float rate) {
    return Kp * angle + Kd * rate;
}

void Controller::setTunings(float kp, float kd) {
    Kp = kp;
    Kd = kd;
}

float* Controller::getKpPointer() {
    return &Kp;
}

float* Controller::getKdPointer() {
    return &Kd;
}
