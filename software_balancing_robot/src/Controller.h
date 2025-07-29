#ifndef CONTROLLER_H
#define CONTROLLER_H

class Controller {
public:
    Controller(float kp, float kd);
    float compute(float angle, float rate);
    void setTunings(float kp, float kd);
    float* getKpPointer();
    float* getKdPointer();

private:
    float Kp, Kd;
};

#endif
