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


    void badRequest() {
      server->send(400, MIME_JSON, "{\"status\": \"bad_request\"}");
    }

    Property *findProperty(String name)
    {
      for (Property *prop : properties)
        if (prop->name == name)
          return prop;

      return NULL;
    }
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
        server->send(200, MIME_JSON, doc.as<String>());
      });

      server->on("/ping", HTTP_GET, [&]() {
        server->send(200, MIME_JSON, "{\"status\": \"ok\"}");
      });


      server->on("/prop", HTTP_GET, [&]() {
        // Example: http://(ip)/prop?name=color&r=5&g=151&b=39

        int totalArgs = server->args();

        if (totalArgs == 0) {
          int capacity = JSON_OBJECT_SIZE(properties.size());
          DynamicJsonDocument doc(capacity);
          for (Property* prop : properties)
            doc.add(prop->name);

          server->send(200, MIME_JSON, doc.as<String>());
        } else {
          if (!server->hasArg("name")) {
            badRequest();
            return;
          }

          String propName = server->arg("name");
          Property *property = findProperty(propName);

          if (property == NULL) {
            badRequest();
            return;
          }

          std::vector<Arg> args;
          for (int i = 0; i < totalArgs; i++) {
            String key = server->argName(i);
            if (key == "name") continue; // The name property is reserved

            String val = server->arg(i);
            
            args.push_back(Arg(key, val));
          }

          Request request(args);
          bool result = false;

          try {
            property->handler(request);
          } catch (const std::runtime_error &e) {
            server->send(500, MIME_JSON, "{\"status\": \"server_error\"}");
            return;
          } catch (const std::invalid_argument &e) {
            badRequest();
            return;
          }

          if (!result) {
            badRequest();
            return;
          }

          server->send(200, MIME_JSON, "{\"status\": \"ok\"}");
        }
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
