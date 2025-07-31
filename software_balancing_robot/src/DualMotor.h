#ifndef DUALMOTOR_H
#define DUALMOTOR_H

#include "Motor.h"

class DualMotor {
public:
    DualMotor(Motor& left, Motor& right);
    
    void begin();
    void setSpeed(float speed);                      // gleiches Tempo für beide
    void setDifferential(float left, float right);   // für differenzielles Fahren
    void stop();
    long getAveragePosition() const;
    void resetPositions();

private:
    Motor& leftMotor;
    Motor& rightMotor;
};

#endif