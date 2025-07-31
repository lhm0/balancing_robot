#include "WebInterface.h"

void WebInterface::connectToWiFi(const char* ssid, const char* password) {
    Serial.print("Verbinde mit WLAN: ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    int retries = 0;
    while (WiFi.status() != WL_CONNECTED && retries < 20) {
        delay(500);
        Serial.print(".");
        retries++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("");
        Serial.print("Verbunden! IP-Adresse: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\nWLAN-Verbindung fehlgeschlagen!");
    }
}

void WebInterface::begin() {
    if (!LittleFS.begin()) {
        Serial.println("LittleFS Mount Failed");
        return;
    }

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(LittleFS, "/index.html", "text/html");
    });

    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(LittleFS, "/style.css", "text/css");
    });

    server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(LittleFS, "/script.js", "application/javascript");
    });

    server.on("/params", HTTP_GET, [this](AsyncWebServerRequest *request){
        String json = "{\"kp\":" + String(*KpRef, 2) + ",\"ki\":" + String(*KiRef, 2) + ",\"kd\":" + String(*KdRef, 2) + "}";
        request->send(200, "application/json", json);
    });

    server.on("/params", HTTP_POST, [this](AsyncWebServerRequest *request){},
        nullptr,
        [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t, size_t) {
        String body = String((char*)data).substring(0, len);
        DynamicJsonDocument doc(256);
        DeserializationError err = deserializeJson(doc, body);
        if (err) {
            request->send(400, "text/plain", "JSON parse error");
            return;
        }
        if (doc.containsKey("kp") && doc.containsKey("ki") && doc.containsKey("kd")) {
            *KpRef = doc["kp"].as<float>();
            *KiRef = doc["ki"].as<float>(); 
            *KdRef = doc["kd"].as<float>();

            request->send(200, "text/plain", "OK");
        } else {
            request->send(400, "text/plain", "Invalid keys");
        }
    });

    server.on("/angle", HTTP_GET, [this](AsyncWebServerRequest* request) {
        if (imuRef) {
            float angle = imuRef->getAngleX();
            String json = "{\"angle\": " + String(angle, 2) + "}";
            request->send(200, "application/json", json);
        } else {
            request->send(500, "application/json", "{\"error\": \"No IMU reference\"}");
        }
    });

    server.on("/resetPosition", HTTP_POST, [this](AsyncWebServerRequest *request){
        if (motorRef && controllerRef) {
            long pos = motorRef->getAveragePosition();
//            controllerRef->setDesiredPosition(pos);
            request->send(200, "text/plain", "Position reset");
        } else {
            request->send(500, "text/plain", "Fehlende Referenzen");
        }
    });    
    
    server.on("/offset", HTTP_POST, [this](AsyncWebServerRequest *request){},
        nullptr,
        [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t, size_t) {
        if (!imuRef) {
            request->send(500, "text/plain", "No IMU reference");
            return;
        }

        String body = String((char*)data).substring(0, len);
        DynamicJsonDocument doc(128);
        if (deserializeJson(doc, body)) {
            request->send(400, "text/plain", "JSON parse error");
            return;
        }

        if (!doc.containsKey("delta")) {
            request->send(400, "text/plain", "Missing 'delta'");
            return;
        }

        float delta = doc["delta"].as<float>();
        imuRef->adjustAngleOffset(delta);
        request->send(200, "text/plain", "Offset angepasst");
    });

    server.begin();
    Serial.println("WebInterface gestartet auf Port 80");
}

void WebInterface::setParameterReference(float* kp, float* ki, float* kd) {
    KpRef = kp;
    KiRef = ki;
    KdRef = kd;
}

void WebInterface::setIMUReference(IMU* imu) {
    imuRef = imu;
}

void WebInterface::setMotorReference(DualMotor* motor) {
    motorRef = motor;
}

void WebInterface::setControllerReference(Controller* controller) {
    controllerRef = controller;
}