#include "Balancer.h"

Balancer::Balancer(IMU& imu, DualMotor& motors, Controller& controller)
    : imu(imu), motors(motors), controller(controller) {}

void Balancer::begin() {
    imu.begin();
    motors.begin();
}

void Balancer::update() {
    imu.update();
    float angle = imu.getAngleX();
    float rate = imu.getGyroRateX();
    float control = controller.compute(angle, rate);
    motors.drive(control);
}
