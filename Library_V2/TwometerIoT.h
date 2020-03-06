#include <Arduino.h>
#include "DeviceDescriptor.h"
#include "WiFiController.h"

#define MIME_JSON "application/json"

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
        wifi.connet();

        server->on("/ping", HTTP_GET, [&]() {
            ok();
        });
    }

    void update() {
        wifi.update();
    }

private:
    void badRequest() {
        server->send(400, MIME_JSON, "{\"status\": \"bad_request\"}");
    }

    void ok() {
        server->send(200, MIME_JSON, "{\"status\": \"ok\"}");
    }
};