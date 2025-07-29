#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

class Motor {
public:
    Motor(int stepPin, int dirPin, int enablePin = -1);
    void begin();
    void setDirection(bool forward);
    void step();
    void enable();
    void disable();

private:
    int stepPin, dirPin, enablePin;
    bool direction = true;
};

#endif
