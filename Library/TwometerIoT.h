#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include "Property.h"
#include "Constants.h"

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

    bool online = false;

  public:
    void describe(DeviceDescriptor desc) {
      this->desc = desc;
    }

    void begin() {
      if (online) return;
      online = true;

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
        ok();
      });

      server->on("/prop", HTTP_GET, [&]() {
        int capacity = JSON_OBJECT_SIZE(properties.size());
        DynamicJsonDocument doc(capacity);
        for (Property* prop : properties)
          doc.add(prop->name);

        server->send(200, MIME_JSON, doc.as<String>());
      });

      for (const Property* prop : properties) {
        server->on("/" + prop->name, HTTP_PUT, [&]() {
          String body = server->arg("plain");
          StaticJsonDocument<200> doc;
          DeserializationError err = deserializeJson(doc, body);

          if (err != DeserializationError::Ok) {
            badRequest();
            return;
          }

          JsonObject obj = doc.as<JsonObject>();

          std::vector<Arg> args;
          for (JsonPair p : obj) {
            String key = String(p.key().c_str());
            JsonVariant val = p.value();
            args.push_back(Arg(key, val));
          }

          Request request(args);
          bool result = false;

          try {
            result = prop->handler(request);
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

          ok();

        });
      }

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

  private:
    void badRequest() {
      server->send(400, MIME_JSON, "{\"status\": \"bad_request\"}");
    }

    void ok() {
      server->send(200, MIME_JSON, "{\"status\": \"ok\"}");
    }
};
