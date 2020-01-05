#include "Hardware.h"
#include "WiFiController.h"
#include "Utils.h"
#include <ESP8266WiFi.h>
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

ConnectedDevice* findDevice(String id) {
  for (ConnectedDevice& dev : connectedDevices)
    if (dev.uuid == id)
      return &dev;

  return NULL;
}

bool isOk(String reply) {
  if (reply == "") return false;
  StaticJsonDocument<JSON_OBJECT_SIZE(1)> doc;
  DeserializationError err = deserializeJson(doc, reply);
  if (err != DeserializationError::Ok || doc["state"] != "ok")
    return false;
  return true;
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

void badGateway(String data) {
  httpServer.send(502, "application/json", data);
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
    controller.EndPair();

    ok(doc.as<String>());
  });

  httpServer.on("/login", HTTP_POST, []() {
    String body = httpServer.arg("plain");
    StaticJsonDocument<JSON_OBJECT_SIZE(1)> doc;
    DeserializationError err = deserializeJson(doc, body);
    if (err != DeserializationError::Ok) {
      badRequest();
      return;
    }

    String uuid = doc["uuid"];
    String ip = httpServer.client().remoteIP().toString();
    connectedDevices.push_back({ uuid, ip });

    ok();
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

    remove(registrationTokens, token);

    String deviceUrl = "http://" + httpServer.client().remoteIP().toString() + "/";
    String deviceData = request(deviceUrl);
    DeviceDescriptor descriptor = parseDeviceDescriptor(deviceData);
    devices.push_back(descriptor);
    ok();
  });

  httpServer.on("/devices", HTTP_POST, []() {
    if (!httpServer.hasArg("id") || !httpServer.hasArg("prop")) {
      badRequest();
      return;
    }

    String devId = httpServer.arg("id");
    String prop = httpServer.arg("prop");
    String payload = httpServer.arg("plain");

    ConnectedDevice* dev = findDevice(devId);
    String url = "http://" + dev->ip + "/" + prop;
    String response = request(url);
    if (isOk(response))
      ok();
    else
      badGateway(response);
  });

  httpServer.on("/devices", HTTP_GET, []() {
    ok();
  });

}

unsigned long long lastCheck = 0;
void loop() {
  httpServer.handleClient();

  unsigned long long now = millis();
  if (now - lastCheck > 30000) {
    std::vector<ConnectedDevice> offlineDevs;
    for (ConnectedDevice& device : connectedDevices) {
      String url = "http://" + device.ip + "/ping";
      String reply = request(url);
      if (!isOk(reply))
        offlineDevs.push_back(device);
    }

    for (int i = 0; i < offlineDevs.size(); i++) {
      ConnectedDevice& device = offlineDevs[i];
      remove(connectedDevices, device);
    }

    lastCheck = now;
  }
}
