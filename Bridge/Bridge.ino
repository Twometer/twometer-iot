#include "Hardware.h"
#include "WiFiController.h"
#include "Utils.h"
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "DeviceDescriptor.h"

const String NAME = "Twometer IoT Bridge";
const String VERSION = "0.1.0";

const String WIFI_SSID = "";
const String WIFI_PASS = "";

std::vector<String> registrationTokens;
std::vector<DeviceDescriptor> devices;
std::vector<ConnectedDevice> connectedDevices;

ESP8266WebServer httpServer(80);
WiFiController controller;

String request(String url) {
  HTTPClient http;
  http.begin(url);

  int code = http.GET();
  if (code == 0) return "";
  String payload = http.getString();
  http.end();
  return payload;
}

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
  WiFi.persistent(true);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }


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
    String body = httpServer.arg("plain");
    StaticJsonDocument<JSON_OBJECT_SIZE(2)> doc;
    DeserializationError err = deserializeJson(doc, body);
    if (err != DeserializationError::Ok) {
      badRequest();
      return;
    }

    String token = doc["token"];
    if (!checkToken(token)) {
      forbidden();
      return;
    }

    registrationTokens.erase(token);

    String deviceUrl = "http://" + httpServer.client().remoteIP() + "/";
    String deviceData = request(deviceUrl);
    DeviceDescriptor descriptor = parseDeviceDescriptor(deviceData);
    devices.push_back(descriptor);
  });

}

unsigned long long lastCheck = 0;
void loop() {
  httpServer.handleClient();

  unsigned long long now = millis();
  if (now - lastCheck > 30000) {
    std::vector<ConnectedDevice&> offlineDevs;
    for (ConnectedDevice& device : connectedDevices) {
      String url = "http://" + device.ip + "/ping";
      String response = request(url);
      StaticJsonDocument<JSON_OBJECT_SIZE(1)> doc;
      DeserializationError err = deserializeJson(doc, body);
      if (err != DeserializationError::Ok || doc["state"] != "ok")
        offlineDevs.push_back(device);
    }

    for (ConnectedDevice& device : offlineDevs)
      connectedDevices.erase(device);

    lastCheck = now;
  }
}
