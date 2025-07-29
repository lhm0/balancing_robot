#ifndef BALANCER_H
#define BALANCER_H

#include "IMU.h"
#include "DualMotor.h"
#include "Controller.h"

class Balancer {
public:
    Balancer(IMU& imu, DualMotor& motors, Controller& controller);
    void begin();
    void update();

private:
    IMU& imu;
    DualMotor& motors;
    Controller& controller;
};

#endif