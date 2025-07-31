#ifndef CONTROLLER_H
#define CONTROLLER_H

class Controller {
public:
    Controller(float kp, float ki, float kd);
    float compute(float angle, float rate); // ⬅ Nur angle und rate
    void setTunings(float kp, float ki, float kd);
    float* getKpPointer();
    float* getKiPointer();
    float* getKdPointer();

private:
    float Kp, Ki, Kd;
    float integral = 0.0f;
};

#endif