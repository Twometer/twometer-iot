#include <EEPROM.h>
#include "Hardware.h"
#include "Utils.h"
#include <ESP8266WiFi.h>

#include "WiFiSettings.h"

const char* WIFI_NAME_PAIR = "Twometer IoT Pair"; // The Pairing WiFi
const char* WIFI_NAME_CTRL = "Twometer IoT"; // The Control WiFi

class WiFiController {
  private:
    bool isPairing;

  public:
    void Initialize() {
      Serial.println("Starting access point...");
      WiFi.softAPdisconnect(true);
      if (STORAGE.wifiKey == "")
      {
        STORAGE.wifiKey = GenerateKey(32);
        storage_write();
      }
      WiFi.softAP(WIFI_NAME_CTRL, STORAGE.wifiKey, 2, 0, 8);
      Connect();

      digitalWrite(LED_ONLINE, HIGH);
    }

    void Connect() {
      Serial.println("Connecting to WiFi...");
      WiFi.persistent(true);
      WiFi.begin(WIFI_SSID, WIFI_PASS);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
      }
      Serial.print("Online at: ");
      Serial.println(WiFi.localIP());
    }

    void BeginPair() {
      Serial.println("Entering pair mode");
      WiFi.softAPdisconnect(true);
      WiFi.softAP(WIFI_NAME_PAIR);
      digitalWrite(LED_PAIRING, HIGH);
      isPairing = true;
    }

    void EndPair() {
      Serial.println("Exiting pair mode");
      Initialize();
      digitalWrite(LED_PAIRING, LOW);
      isPairing = false;
    }

    bool IsPairing() {
      return isPairing;
    }

};
