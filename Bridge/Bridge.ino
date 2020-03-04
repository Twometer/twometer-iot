#include <ArduinoJson.h>
#include "Hardware.h"
#include "DeviceDescriptor.h"
#include "Storage.h"

#include "WiFiController.h"
#include "Utils.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

const char* NAME = "Twometer IoT Bridge";
const char* VERSION = "0.1.0";

bool schedule_exit_pair = false;

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
  for (ConnectedDevice& dev : STORAGE.connectedDevices)
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
  for (String & test : STORAGE.registrationTokens)
    if (token == test)
      return true;
  return false;
}

void setup() {
  // Serial init
  Serial.begin(9600);
  Serial.println("");
  Serial.print(NAME);
  Serial.print("  v");
  Serial.println(VERSION);

  // Hardware init
  pinMode(BTN_PAIR, INPUT_PULLUP);
  pinMode(LED_PAIRING, OUTPUT);
  pinMode(LED_ONLINE, OUTPUT);
  digitalWrite(LED_ONLINE, LOW);
  digitalWrite(LED_PAIRING, LOW);

  Serial.println("Loading settings...");
  storage_read();

  // Register click interrupt
  delay(500);
  attachInterrupt(digitalPinToInterrupt(BTN_PAIR), ClickInterrupt, FALLING);

  // If pair button is presed at startup, enter erase mode
  // This will factory reset the device
  if (digitalRead(BTN_PAIR) == LOW) {
    Serial.println("Entering erase mode..");

    // Blink LEDs for 3 seconds while the user
    // holds down the reset button
    for (int i = 0; i < (3000 / 100); i++) {
      digitalWrite(LED_ONLINE, LOW);
      digitalWrite(LED_PAIRING, LOW);
      delay(50);
      digitalWrite(LED_ONLINE, HIGH);
      digitalWrite(LED_PAIRING, HIGH);
      delay(50);
      if (digitalRead(BTN_PAIR) != LOW) break;
    }

    // If still held, reset and restart
    if (digitalRead(BTN_PAIR) == LOW) {
      Serial.println("Erase confirmed. Erasing ...");
      digitalWrite(LED_ONLINE, HIGH);
      digitalWrite(LED_PAIRING, HIGH);
      delay(50);
      storage_erase();
      Serial.println("Erase complete, waiting for user to let go...");
      digitalWrite(LED_ONLINE, LOW);
      digitalWrite(LED_PAIRING, LOW);
      while (digitalRead(BTN_PAIR) == LOW) delay(100);
      Serial.println("Rebooting");
      ESP.restart();
      return;
    }

    digitalWrite(LED_ONLINE, LOW);
    digitalWrite(LED_PAIRING, LOW);
  }

  // Setup WiFi connection
  controller.Initialize();

  // Register endpoints
  httpServer.on("/", HTTP_ANY , []() {
    StaticJsonDocument<JSON_OBJECT_SIZE(2)> doc;
    doc["name"] = NAME;
    doc["version"] = VERSION;
    ok(doc.as<String>());
  });

  httpServer.on("/keys", HTTP_GET, []() {
    if (!controller.IsPairing() || schedule_exit_pair) {
      forbidden();
      return;
    }

    String token = GenerateKey(32);
    STORAGE.registrationTokens.push_back(token);
    storage_write();

    StaticJsonDocument<JSON_OBJECT_SIZE(2)> doc;
    doc["key"] = STORAGE.wifiKey.c_str();
    doc["token"] = token.c_str();
    ok(doc.as<String>());

    Serial.println("Scheduling Pair Mode Shutdown");

    schedule_exit_pair = true;
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
    STORAGE.connectedDevices.push_back({ uuid, ip });
    storage_write();

    ok();
  });

  httpServer.on("/register", HTTP_POST, []() {
    String body = httpServer.arg("plain");

    DynamicJsonDocument doc(JSON_OBJECT_SIZE(2) + 60);
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

    remove(STORAGE.registrationTokens, token);

    String deviceUrl = "http://" + httpServer.client().remoteIP().toString() + "/";
    String deviceData = request(deviceUrl);
    DeviceDescriptor descriptor = parseDeviceDescriptor(deviceData);
    STORAGE.devices.push_back(descriptor);

    storage_write();
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
    DynamicJsonDocument doc(1024);
    for (DeviceDescriptor& device : STORAGE.devices) {
      JsonObject obj = doc.createNestedObject();
      obj["uuid"] = device.uuid;
      obj["name"] = device.name;
      obj["type"] = device.type;
      obj["manufacturer"] = device.manufacturer;
      doc.add(obj);
    }
    ok(doc.as<String>());
  });

  httpServer.begin();

}

void updateDeviceList() {
  std::vector<ConnectedDevice>::iterator it = STORAGE.connectedDevices.begin();
  while (it != STORAGE.connectedDevices.end()) {
    String url = "http://" + it->ip + "/ping";
    String reply = requestWithTimeout(url, 750);
    if (!isOk(reply))
      it = STORAGE.connectedDevices.erase(it);
    else ++it;
  }
  storage_write();
}

unsigned long long lastCheck = 0;
void loop() {
  httpServer.handleClient();

  if (millis() - lastCheck > 30000) {
    updateDeviceList();
    lastCheck = millis();
  }
  
  if (schedule_exit_pair) {
    schedule_exit_pair = false;
    delay(10000);
    controller.EndPair();
  }
}
