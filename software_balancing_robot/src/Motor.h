#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>

class Motor {
public:
    Motor(int stepPin, int dirPin, int enablePin, bool reversed, int pwmChannel, int stepSensePin);

    void begin();
    void setDirection(bool dir);
    void setSpeed(float stepsPerSecond);  // 0 = stoppen
    void enable();
    void disable();
    float getSpeed() const;
    long getPosition() const;             // 🆕 Schrittzähler (pseudo-Position)
    void resetPosition(); // optional für Offset beim Start

private:
    int stepPin;
    int dirPin;
    int enablePin;
    bool reversed;
    int pwmChannel;
    int stepSensePin;

    volatile long position = 0;
    bool currentDirection = true;

    static void IRAM_ATTR onStepISR0();  // ISR für Motor 0
    static void IRAM_ATTR onStepISR1();  // ISR für Motor 1

    void handleStepISR();                // interne Zähler-Logik

    static Motor* instance0;
    static Motor* instance1;
};

#endif