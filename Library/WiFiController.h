#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <EEPROM.h>
#include "StorageBuf.h"
#include "HttpLib.h"

#define EEPROM_INIT_MAGIC 42

const char* WIFI_NAME_PAIR = "Twometer IoT Pair"; // The Pairing WiFi
const char* WIFI_NAME_CTRL = "Twometer IoT"; // The Control WiFi

class WiFiController
{
  public:

    void Connect(DeviceDescriptor& desc) {
      WiFi.persistent(false); // We handle the reconnect ourselves
      WiFi.mode(WIFI_STA);

      EEPROM.begin(256);
      StorageBuf buf;
      bool eeprom_has_data = buf.read_byte() == EEPROM_INIT_MAGIC;

      if (eeprom_has_data) {
        Serial.println("EEPROM has data, connecting...");

        wifiKey = buf.read_string();
        WiFi.begin(WIFI_NAME_CTRL, wifiKey);

        Serial.println("Device id: " + String(ESP.getChipId(), HEX));

        AwaitConnected();

        Login();
      } else {
        Serial.println("EEPROM has no data, registering...");

        WiFi.begin(WIFI_NAME_PAIR);
        AwaitConnected();

        GetKey();
        WiFi.begin(WIFI_NAME_CTRL, wifiKey);
        AwaitConnected();

        delay(5000);

        Register(desc);
        Login();
      }

      Serial.println("WiFi setup complete");
      initComplete = true;
    }

    void Update() {
      if (initComplete) {
        if (lastWifiState != WL_CONNECTED && WiFi.status() == WL_CONNECTED)
        {
          delay(1000);
          Serial.println("WiFi Connection regained, authenticating with bridge...");
          Login();
          Serial.println("Done");
        }
        if (lastWifiState == WL_CONNECTED && WiFi.status() != WL_CONNECTED) {
          Serial.println("WiFi Connection lost");
        }

        lastWifiState = WiFi.status();
      }
    }

  private:
    String wifiKey;
    String registerToken;
    int lastWifiState = WL_CONNECTED;
    bool initComplete = false;

    void Login() {
      Serial.println("Login...");

      String bridgeIp = WiFi.gatewayIP().toString();
      String url = "http://" + bridgeIp + "/login";

      HttpResponse response;
      do {
        response = http_post(url, "{\"uuid\": \"" + String(ESP.getChipId(), HEX) + "\"}");
        if (response.code == 403)
        {
          // 403 forbidden
          // Registration has changed, clear and reboot
          Serial.println("Bridge does not know this device but key is correct, resetting...");
          for (int i = 0; i < 256; i++)
            EEPROM.write(i, 255);
          EEPROM.commit();
          ESP.restart();
          return;
        }
      } while (response.code != 200);
    }

    void GetKey() {
      Serial.println("Authenticating...");

      String bridgeIp = WiFi.gatewayIP().toString();
      String url = "http://" + bridgeIp + "/keys";

      HttpResponse response = http_get(url);
      if (response.code == 200)
      {
        StaticJsonDocument < JSON_OBJECT_SIZE(2) + 90 > doc;
        DeserializationError err = deserializeJson(doc, response.data);
        if (err != DeserializationError::Ok) {
          return;
        }
        JsonObject obj = doc.as<JsonObject>();

        this->wifiKey = obj["key"].as<String>();
        this->registerToken = obj["token"].as<String>();

        Serial.println("Wifi key: " + this->wifiKey);
        Serial.println("Register token: " + this->registerToken);

        StorageBuf buf;
        buf.write_byte(EEPROM_INIT_MAGIC);
        buf.write_string(this->wifiKey);
        EEPROM.commit();
      }
    }

    void Register(DeviceDescriptor& desc) {
      Serial.println("Registering...");

      int strLen = desc.name.length() + 1 + desc.type.length() + 1 + desc.manufacturer.length() + 1 + this->registerToken.length() + 1 + 30;
      DynamicJsonDocument doc(JSON_OBJECT_SIZE(5) + strLen);
      doc["uuid"] = desc.uuid;
      doc["name"] = desc.name;
      doc["type"] = desc.type;
      doc["manufacturer"] = desc.manufacturer;
      doc["token"] = this->registerToken;
      String payload;
      serializeJson(doc, payload);

      Serial.println(payload);



      String bridgeIp = WiFi.gatewayIP().toString();
      String url = "http://" + bridgeIp + "/register";

      HttpResponse response;
      do {
        response = http_post(url, payload);
        Serial.println("Reply: " + String(response.code));
      } while (response.code != 200);
    }

    void AwaitConnected() {
      Serial.println("Connecting to " + WiFi.SSID() + " ...");
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
      }
    }

};
