#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <EEPROM.h>
#include "StorageBuf.h"

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

        Serial.println("WiFi key: " + wifiKey);

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
    }

  private:
    String wifiKey;
    String registerToken;

    void Login() {
      Serial.println("Login...");

      String bridgeIp = WiFi.gatewayIP().toString();
      String url = "http://" + bridgeIp + "/login";

      WiFiClient wifi;
      HTTPClient http;
      http.begin(wifi, url);
      http.POST("{\"uuid\": \"" + String(ESP.getChipId(), HEX) + "\"}");
      http.end();
    }

    void GetKey() {
      Serial.println("Authenticating...");

      String bridgeIp = WiFi.gatewayIP().toString();
      String url = "http://" + bridgeIp + ":80/keys";

      WiFiClient wifi;
      HTTPClient http;
      http.begin(wifi, url);

      int code = http.GET();
      if (code > 0) {
        String payload = http.getString();

        StaticJsonDocument < JSON_OBJECT_SIZE(2) + 90 > doc;
        DeserializationError err = deserializeJson(doc, payload);
        if (err != DeserializationError::Ok) {
          return;
        }
        JsonObject obj = doc.as<JsonObject>();

        this->wifiKey = obj["key"].as<String>();
        this->registerToken = obj["token"].as<String>();

        StorageBuf buf;
        buf.write_byte(EEPROM_INIT_MAGIC);
        buf.write_string(this->wifiKey);
        EEPROM.commit();
      }
      http.end();
    }

    void Register(DeviceDescriptor& desc) {
      Serial.println("Registering...");

      StaticJsonDocument < JSON_OBJECT_SIZE(5) > doc;
      doc["uuid"] = desc.uuid;
      doc["name"] = desc.name;
      doc["type"] = desc.type;
      doc["manufacturer"] = desc.manufacturer;
      doc["token"] = registerToken;
      String payload = doc.as<String>();

      String bridgeIp = WiFi.gatewayIP().toString();
      String url = "http://" + bridgeIp + ":80/register";

      WiFiClient wifi;
      HTTPClient http;
      http.begin(wifi, url);
      http.addHeader("Content-Type", "application/json");

      http.POST(payload);
      http.end();
    }

    void AwaitConnected() {
      Serial.println("Connecting...");
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
      }
    }

};
