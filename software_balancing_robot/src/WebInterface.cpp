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
        String json = "{\"kp\":" + String(*KpRef, 2) + ",\"kd\":" + String(*KdRef, 2) + "}";
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
        if (doc.containsKey("kp") && doc.containsKey("kd")) {
            *KpRef = doc["kp"].as<float>();
            *KdRef = doc["kd"].as<float>();
            request->send(200, "text/plain", "OK");
        } else {
            request->send(400, "text/plain", "Invalid keys");
        }
    });

    server.begin();
    Serial.println("WebInterface gestartet auf Port 80");
}

void WebInterface::setParameterReference(float* kp, float* kd) {
    KpRef = kp;
    KdRef = kd;
}
