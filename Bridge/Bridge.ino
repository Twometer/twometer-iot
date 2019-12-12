#include <ESP8266WiFi.h>
WiFiServer server(80);

bool is_pairing = false;

void ICACHE_RAM_ATTR ClickInterrupt() {
  static unsigned long last_interrupt_time = 500;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 200)
  {
    if (!is_pairing)
    {
      WiFi.softAPdisconnect(true);
      WiFi.softAP("Twometer IoT Pair");
      digitalWrite(16, LOW);
      digitalWrite(14, HIGH);
      is_pairing = true;
    } else {
      WiFi.softAPdisconnect(true);
      WiFi.softAP("Twometer IoT", "9172391264");
      digitalWrite(16, HIGH);
       digitalWrite(14, LOW);
      is_pairing = false;
    }
  }
  last_interrupt_time = interrupt_time;
}

void setup() {
  pinMode(12, INPUT_PULLUP);
  pinMode(14, OUTPUT);
  pinMode(16, OUTPUT);
  digitalWrite(16, LOW);
  digitalWrite(14, LOW);

  delay(500);
  attachInterrupt(digitalPinToInterrupt(12), ClickInterrupt, FALLING);
  WiFi.softAPdisconnect(true);
  WiFi.softAP("Twometer IoT", "9172391264");

  digitalWrite(16, HIGH);

}

void loop() {

}
