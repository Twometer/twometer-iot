#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include "Property.h"

#define TYPE_LIGHT_GENERIC "lights/generic"
#define TYPE_LIGHT_STRIPE "lights/strip"
#define TYPE_SWITCH "switch/simple"
#define TYPE_MULTISWITCH "switch/multi"

#define MIME_JSON "application/json"

struct DeviceDescriptor {
  String name;
  String type;
  String manufacturer;
};

class TwometerIoT {
  private:
    DeviceDescriptor desc;

    ESP8266WebServer* server;

    std::vector<Property*> properties;

  public:
    void describe(DeviceDescriptor desc) {
      this->desc = desc;
    }

    void begin() {
      this->server = new ESP8266WebServer(80);

      server->on("/", HTTP_GET, [&]() {
        const int capacity = JSON_OBJECT_SIZE(3);
        StaticJsonDocument<capacity> doc;
        doc["name"] = desc.name;
        doc["type"] = desc.type;
        doc["manufacturer"] = desc.manufacturer;
        
        String output;
        serializeJson(doc, output);
        
        server->send(200, MIME_JSON, output);
      });

      server->on("/ping", HTTP_GET, [&]() {
        server->send(200, MIME_JSON, "{\"state\": \"online\"}");
      });

      server->begin();
    }

    Property& prop(String name) {
      Property* property = new Property(name);
      properties.push_back(property);
      return *property;
    }

    void update() {
      server->handleClient();
    }


};
