#ifndef WEBINTERFACE_H
#define WEBINTERFACE_H

#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "IMU.h" 
#include "DualMotor.h"
#include "Controller.h"

class WebInterface {
public:
    void begin();
    void setParameterReference(float* kp, float* ki, float* kd);
    void setIMUReference(IMU* imu); 
    void connectToWiFi(const char* ssid, const char* password);
    void setMotorReference(DualMotor* motor);
    void setControllerReference(Controller* controller); 

private:
    AsyncWebServer server{80};
    float* KpRef = nullptr;
    float* KiRef = nullptr;
    float* KdRef = nullptr;

    DualMotor* motorRef = nullptr;
    Controller* controllerRef = nullptr;

    IMU* imuRef = nullptr;
};

#endif