#ifndef WEBINTERFACE_H
#define WEBINTERFACE_H

#include <Arduino.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <WiFi.h>
#include <ArduinoJson.h>

class WebInterface {
public:
    void begin();
    void setParameterReference(float* kp, float* kd);
    void connectToWiFi(const char* ssid, const char* password);

private:
    AsyncWebServer server{80};
    float* KpRef = nullptr;
    float* KdRef = nullptr;
};

#endif