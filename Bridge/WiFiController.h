#include <EEPROM.h>
#include "Hardware.h"
#include <ESP8266WiFi.h>

const char* WIFI_NAME_PAIR = "Twometer IoT Pair"; // The Pairing WiFi
const char* WIFI_NAME_CTRL = "Twometer IoT"; // The Control WiFi

class WiFiController {
  private:
    const char* key;
    bool isPairing;

    const char* GenKey(int len) {
      char* key = new char[len];
      for (int i = 0; i < len; i++) {
        char c = 'a' + rand() * ('z' - 'a');
        key[i] = c;
      }
      return key;
    }

  public:
    void Initialize() {
      WiFi.softAPdisconnect(true);
      WiFi.softAP(WIFI_NAME_CTRL, key);
      digitalWrite(LED_ONLINE, HIGH);
    }

    void BeginPair() {
      WiFi.softAPdisconnect(true);
      WiFi.softAP(WIFI_NAME_PAIR);
      digitalWrite(LED_PAIRING, HIGH);
      isPairing = true;
    }

    void EndPair() {
      Initialize();
      digitalWrite(LED_PAIRING, LOW);
      isPairing = false;
    }

    bool IsPairing() {
      return isPairing;
    }

    const char* GetKey() {
      return key;
    }

};
