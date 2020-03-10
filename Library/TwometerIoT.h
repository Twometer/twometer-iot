#ifndef twometeriot_h
#define twometeriot_h

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include "DeviceDescriptor.h"
#include "WiFiController.h"
#include "Property.h"
#include "ModeProperty.h"
#include "Types.h"

#define MIME_JSON "application/json"
#define LOCALE "de-DE"

class TwometerIoT {
private:
    WiFiController wifi;
    DeviceDescriptor descriptor;

    ESP8266WebServer* server;

    std::vector<Property*> properties;

    unsigned long long lastPing = 0;

public:
    void describe(String modelName, String manufacturer, String description, String type) {
        String chipId = String(ESP.getChipId(), HEX);
        this->descriptor = { chipId, modelName, manufacturer, description, type };
    }

    ModeProperty &handleMode(String name, std::function<bool(const DynamicJsonDocument&)> handler) {
        ModeProperty *property = new ModeProperty();
        property->propertyType = PROPERTY_MODE;
        property->name = name;
        property->handler = handler;
        properties.push_back(property);
        return *property;
    }

    Property &handle(String name, std::function<bool(const DynamicJsonDocument&)> handler) {
        Property *property = new Property();
        property->propertyType = PROPERTY_REGULAR;
        property->name = name;
        property->handler = handler;
        properties.push_back(property);
        return *property;
    }

    void begin() {
        Serial.begin(9600);
        Serial.println("Twometer IoT Client Library v2");

        wifi.connect(this->descriptor);
        this->server = new ESP8266WebServer(80);
        this->lastPing = millis(); // Initialize last ping variable to avoid login spam before the WiFi is even connected

        server->on("/ping", HTTP_GET, [&]() {
            lastPing = millis();
            ok();
        });

        server->on("/set", HTTP_POST, [&]() {
            if (!server->hasArg("prop")) {
                badRequest();
                return;
            }

            String key = server->arg("prop");
            String data = server->arg("plain");

            Property *property = getProperty(key);
            if (property == NULL) {
                notFound();
                return;
            }

            DynamicJsonDocument *doc = new DynamicJsonDocument(256);
            if (deserializeJson(*doc, data) != DeserializationError::Ok) {
                badRequest();
                return;
            }

            bool result = property->handler(*doc);

            if (result) {
                delete property->currentState;
                property->currentState = doc;
                ok();
            } else {
                delete doc;
                badRequest();
            }
        });

        server->on("/get", HTTP_GET, [&]() {
            if (!server->hasArg("prop")) {
                badRequest();
                return;
            }

            String key = server->arg("prop");
            Property *property = getProperty(key);
            if (property == NULL) {
                notFound();
                return;
            }

            DynamicJsonDocument *doc = property->currentState;
            if (doc == NULL) {
                server->send(200, MIME_JSON, "{}");
            } else {
                server->send(200, MIME_JSON, doc->as<String>());
            }
        });

        server->on("/capabilities", HTTP_GET, [&]() {
            // Calculate capacity of the JSON document
            int capacity = JSON_OBJECT_SIZE(properties.size());
            for (Property* prop : properties) {
                if(prop->propertyType == PROPERTY_MODE) {
                    ModeProperty* modeProp = (ModeProperty*) prop;
                    capacity += JSON_OBJECT_SIZE(modeProp->entries.size() + 3) + 64; // Basic properties
                    for (ModeEntry &entry : modeProp->entries) // Extended properties
                      capacity += (entry.key.length() + entry.value.length());
                } else {
                    capacity += JSON_OBJECT_SIZE(2) + 32;
                }
            }

            // Build JSON document
            DynamicJsonDocument doc(capacity);
            for (Property* prop : properties) {
                if(prop->propertyType == PROPERTY_MODE) {
                    ModeProperty* modeProp = (ModeProperty*) prop;

                    JsonObject obj = doc.createNestedObject();
                    obj["name"] = prop->name;
                    obj["type"] = "mode";
                    obj["friendlyName"] = modeProp->friendlyName;
                    for (ModeEntry &entry : modeProp->entries)
                        obj[entry.key] = entry.value;
                } else {
                    JsonObject obj = doc.createNestedObject();
                    obj["name"] = prop->name;
                    obj["type"] = "regular";
                }
            }

            server->send(200, MIME_JSON, doc.as<String>());
        });

        server->begin();
    }

    void update() {
        server->handleClient();
        wifi.update();

        if (millis() - lastPing > 60000) { // No ping for more than a minute (normally pings arrive every 30 secs)
            wifi.reconnect(); // Upstream connection lost, try reconnecting
            lastPing = millis(); // Reset the last ping state to avoid login spam
        }
    }

private:
    Property* getProperty(String key) {
        for (Property *prop : properties)
            if (prop->name == key)
                return prop;
        return NULL;
    }

    void ok() {
        server->send(200, MIME_JSON, "{\"status\": \"ok\"}");
    }

    void badRequest() {
        server->send(400, MIME_JSON, "{\"status\": \"bad_request\"}");
    }

    void notFound() {
        server->send(404, MIME_JSON, "{\"status\": \"not_found\"}");
    }
};

#endif
