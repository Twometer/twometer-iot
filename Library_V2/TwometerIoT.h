#include <Arduino.h>
#include "DeviceDescriptor.h"
#include "WiFiController.h"

#define MIME_JSON "application/json"
#define LOCALE "de-DE"

class TwometerIoT {
private:
    WiFiController wifi;
    DeviceDescriptor descriptor;

    ESP8266WebServer* server;

public:

    void describe(String modelName, String manufacturer, String description, String type) {
        String chipId = String(ESP.getChipId(), HEX);
        this->descriptor = { chipId, modelName, manufacturer, description, type };
    }

    void begin() {
        Serial.begin(9600);
        Serial.println("Twometer IoT Client Library v2");

        wifi.connect();
        this->server = new ESP8266WebServer(80);

        server->on("/ping", HTTP_GET, [&]() {
            ok();
        });

        server->begin();
    }

    void update() {
        server->handleClient();
        wifi.update();
    }

private:
    void ok() {
        server->send(200, MIME_JSON, "{\"status\": \"ok\"}");
    }

    void badRequest() {
        server->send(400, MIME_JSON, "{\"status\": \"bad_request\"}");
    }
};