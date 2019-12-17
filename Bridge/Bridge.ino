#include "Hardware.h"
#include "WiFiController.h"
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>

ESP8266WebServer httpServer(80);
WiFiController controller;

void ICACHE_RAM_ATTR ClickInterrupt() {
  static unsigned long lastInterrupt = 500;
  unsigned long now = millis();
  if (now - lastInterrupt > 200)
  {
    if (!controller.IsPairing())
      controller.BeginPair();
    else
      controller.EndPair();
  }
  lastInterrupt = now;
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
    httpServer.send(200, "text/html", "console here");
  });

  // TODO Rest APi

}

void loop() {

}
