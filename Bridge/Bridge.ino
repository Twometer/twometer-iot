#include <ArduinoJson.h>
#include "Hardware.h"
#include "DeviceDescriptor.h"
#include "Storage.h"

#include "WiFiController.h"
#include "Utils.h"
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <WiFiUdp.h>
#include "HttpLib.h"

#define UDP_PORT 38711
#define HEARTBEAT_MISS_THRESHOLD 2

const char* NAME = "Twometer IoT Bridge";
const char* VERSION = "2.0.1";

unsigned long long lastCheck = 0;
bool schedule_exit_pair = false;
char udp_incoming[255];

ESP8266WebServer httpServer(80);
WiFiController controller;
WiFiUDP udp;

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
  StaticJsonDocument < JSON_OBJECT_SIZE(1) + 32 > doc;
  DeserializationError err = deserializeJson(doc, reply);
  if (err != DeserializationError::Ok || doc["status"] != "ok")
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

void notFound() {
  httpServer.send(404, "application/json", "{\"status\": \"not_found\"}");
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

bool deviceExists(String uuid) {
  for (DeviceDescriptor& desc : STORAGE.devices)
    if (desc.deviceId == uuid)
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
  udp.begin(UDP_PORT);

  /* Register endpoints */

  // Info endpoint
  httpServer.on("/", HTTP_ANY , []() {
    StaticJsonDocument<JSON_OBJECT_SIZE(2)> doc;
    doc["name"] = NAME;
    doc["version"] = VERSION;
    ok(doc.as<String>());
  });

  // Debug endpoint
  httpServer.on("/debug", HTTP_GET, []() {
    StaticJsonDocument < JSON_OBJECT_SIZE(11) + 32 > doc;
    doc["name"] = NAME;
    doc["version"] = VERSION;
    doc["registry_token_count"] = STORAGE.registrationTokens.size();
    doc["connected_device_count"] = STORAGE.connectedDevices.size();
    doc["registered_device_count"] = STORAGE.devices.size();
    doc["mode"] = controller.IsPairing() ? "pairing" : "regular";
    doc["wifi_ssid"] = WiFi.SSID();
    doc["wifi_rssi"] = WiFi.RSSI();
    doc["system_time"] = millis();
    doc["last_ping_stime"] = lastCheck;
    doc["pair_shutdown_sched"] = schedule_exit_pair;
    ok(doc.as<String>());
  });

  // Authorization
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

  // Logging in
  httpServer.on("/login", HTTP_POST, []() {
    String body = httpServer.arg("plain");
    StaticJsonDocument < JSON_OBJECT_SIZE(1) + 50 > doc;
    DeserializationError err = deserializeJson(doc, body);
    if (err != DeserializationError::Ok) {
      badRequest();
      return;
    }

    String uuid = doc["uuid"];
    if (!deviceExists(uuid)) {
      forbidden();
      return;
    }

    String ip = httpServer.client().remoteIP().toString();
    STORAGE.connectedDevices.push_back({ uuid, ip });
    storage_write();

    Serial.println(ip + " logged in");

    ok();
  });

  // Registering
  httpServer.on("/register", HTTP_POST, []() {
    String body = httpServer.arg("plain");

    DynamicJsonDocument doc(JSON_OBJECT_SIZE(6) + 256);
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

    DeviceDescriptor descriptor = parseDeviceDescriptor(doc);
    STORAGE.devices.push_back(descriptor);

    storage_write();
    ok();
  });

  // Setting name
  httpServer.on("/name", HTTP_POST, []() {
    if (!httpServer.hasArg("id") || !httpServer.hasArg("name")) {
      badRequest();
      return;
    }

    String devId = httpServer.arg("id");
    String name = httpServer.arg("name");
    bool found = false;

    for (DeviceDescriptor &desc : STORAGE.devices)
      if (desc.deviceId == devId) {
        desc.friendlyName = name;
        found = true;
      }

    if (!found) {
      notFound();
      return;
    }

    storage_write();
    ok();
  });

  // Set property on a device
  httpServer.on("/set", HTTP_POST, []() {
    if (!httpServer.hasArg("id") || !httpServer.hasArg("prop")) {
      badRequest();
      return;
    }

    String devId = httpServer.arg("id");
    String prop = httpServer.arg("prop");
    String payload = httpServer.arg("plain");

    ConnectedDevice* dev = findDevice(devId);
    if (dev == NULL) {
      badGateway("{\"status\": \"device_offline\"}");
      return;
    }
    String url = "http://" + dev->ip + "/set?prop=" + prop;
    HttpResponse response = http_post(url, payload);

    if (response.code == 200)
      ok(response.data);
    else
      badGateway(response.data);
  });

  // Get a property from a device
  httpServer.on("/get", HTTP_GET, []() {
    if (!httpServer.hasArg("id") || !httpServer.hasArg("prop")) {
      badRequest();
      return;
    }

    String devId = httpServer.arg("id");
    String prop = httpServer.arg("prop");

    ConnectedDevice* dev = findDevice(devId);
    if (dev == NULL) {
      badGateway("{\"status\": \"device_offline\"}");
      return;
    }
    String url = "http://" + dev->ip + "/get?prop=" + prop;
    HttpResponse response = http_get(url);

    if (response.code == 200)
      ok(response.data);
    else
      badGateway(response.data);
  });

  // Get device capabilities
  httpServer.on("/capabilities", HTTP_GET, []() {
    if (!httpServer.hasArg("id")) {
      badRequest();
      return;
    }

    String devId = httpServer.arg("id");
    String payload = httpServer.arg("plain");

    ConnectedDevice* dev = findDevice(devId);
    if (dev == NULL) {
      badGateway("{\"status\": \"device_offline\"}");
      return;
    }
    String url = "http://" + dev->ip + "/capabilities";
    HttpResponse response = http_get(url);
    if (response.code != 200) {
      badGateway("{\"status\": \"device_offline\"}");
      return;
    }

    ok(response.data);
  });

  // List all devices
  httpServer.on("/devices", HTTP_GET, []() {
    DynamicJsonDocument doc(256 * STORAGE.devices.size());
    for (DeviceDescriptor& device : STORAGE.devices) {
      JsonObject obj = doc.createNestedObject();
      obj["deviceId"] = device.deviceId;
      obj["modelName"] = device.modelName;
      obj["manufacturer"] = device.manufacturer;
      obj["description"] = device.description;
      obj["type"] = device.type;
      if (device.friendlyName != "")
        obj["friendly_name"] = device.friendlyName;

      bool isOnline = findDevice(device.deviceId) != NULL;
      obj["online"] = isOnline;
    }
    ok(doc.as<String>());
  });

  httpServer.begin();

}

void updateDeviceList() {
  Serial.println("* Begin Device list update *");
  std::vector<ConnectedDevice>::iterator it = STORAGE.connectedDevices.begin();
  while (it != STORAGE.connectedDevices.end()) {
    String url = "http://" + it->ip + "/ping";
    String reply = requestWithTimeout(url, 1250);
    Serial.println("Reply: " + reply);
    if (!isOk(reply)) {
      it->missedHeartbeats++;
      if (it->missedHeartbeats > HEARTBEAT_MISS_THRESHOLD)
        it = STORAGE.connectedDevices.erase(it);
    } else {     
      it->missedHeartbeats = 0;
      ++it;
    }
  }
  storage_write();
  Serial.println("* End device list update *");
}

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

  int packetSize = udp.parsePacket();
  if (packetSize)
  {
    int len = udp.read(udp_incoming, 255);
    if (len > 0)
      udp_incoming[len] = 0;

    // Trigger sequence 0x00 0x42 0x69
    // Very mature I know
    if (udp_incoming[0] == 0x00 && udp_incoming[1] == 0x42 && udp_incoming[2] == 0x69)
    {
      String myIp = WiFi.localIP().toString();
      udp.beginPacket(udp.remoteIP(), udp.remotePort());
      udp.print(myIp);
      udp.endPacket();
    }
  }
}
