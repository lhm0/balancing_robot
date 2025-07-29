#ifndef DUALMOTOR_H
#define DUALMOTOR_H

#include "Motor.h"

class DualMotor {
public:
    DualMotor(Motor& left, Motor& right);
    void begin();
    void drive(float speed);
    void driveDifferential(float leftSpeed, float rightSpeed);
    void stop();

private:
    Motor& leftMotor;
    Motor& rightMotor;
};

#endif
