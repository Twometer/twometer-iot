#include "Hardware.h"
#include "WiFiController.h"
#include "Utils.h"
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

const String NAME = "Twometer IoT Bridge";
const String VERSION = "0.1.0";

std::vector<String> registrationTokens;
ESP8266WebServer httpServer(80);
WiFiController controller;

void ICACHE_RAM_ATTR ClickInterrupt() {
  static unsigned long lastInterrupt = 500;
  unsigned long now = millis();
  if (now - lastInterrupt > 200) {
    if (!controller.IsPairing())
      controller.BeginPair();
    else
      controller.EndPair();
  }
  lastInterrupt = now;
}

void ok() {
  httpServer.send(200, "application/json", "{\"status\": \"ok\"}");
}

void ok(String val) {
  httpServer.send(200, "application/json", val);
}

void badRequest() {
  httpServer.send(400, "application/json", "{\"status\": \"bad_request\"}");
}

void forbidden() {
  httpServer.send(403, "application/json", "{\"status\": \"forbidden\"}");
}

void serverError() {
  httpServer.send(500, "application/json", "{\"status\": \"error\"}");
}

bool checkToken(String token) {
  for (String & test : registrationTokens)
    if (token == test)
      return true;
  return false;
}

void setup() {
  pinMode(BTN_PAIR, INPUT_PULLUP);
  pinMode(LED_PAIRING, OUTPUT);
  pinMode(LED_ONLINE, OUTPUT);
  digitalWrite(LED_ONLINE, LOW);
  digitalWrite(LED_PAIRING, LOW);

  delay(500);
  attachInterrupt(digitalPinToInterrupt(BTN_PAIR), ClickInterrupt, FALLING);
  controller.Initialize();

  httpServer.on("/", HTTP_GET, []() {
    StaticJsonDocument<JSON_OBJECT_SIZE(2)> doc;
    doc["name"] = NAME;
    doc["version"] = VERSION;
    ok(doc.as<String>());
  });

  httpServer.on("/keys", HTTP_GET, []() {
    if (!controller.IsPairing()) {
      forbidden();
      return;
    }

    String token = GenerateKey(32);
    registrationTokens.push_back(token);

    StaticJsonDocument<JSON_OBJECT_SIZE(2)> doc;
    doc["key"] = controller.GetKey();
    doc["token"] = token;
    ok(doc.as<String>());

    controller.EndPair();
  });

  httpServer.on("/register", HTTP_POST, []() {
    String body = server.arg("body");
    StaticJsonDocument<JSON_OBJECT_SIZE(2)> doc;
    DeserializationError err = deserializeJson(doc, body);
    if (err != DeserializationError::Ok) {
      badRequest();
      return;
    }

    String token = doc["token"];
    if (!checkToken(token)) {
      forbidden()
      return;
    }
  });

}

void loop() {
  httpServer.handleClient();
}
