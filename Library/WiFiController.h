#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <EEPROM.h>

const char* WIFI_NAME_PAIR = "Twometer IoT Pair"; // The Pairing WiFi
const char* WIFI_NAME_CTRL = "Twometer IoT"; // The Control WiFi

struct WifiData {
  bool hasKeys = false;
  String ssid = "";
  String key = "";
  String token = "";
};

class WiFiController {

  public:

    void Connect() {
      WiFi.persistent(false); // We handle the reconnect ourselves

      EEPROM.begin(512);

      WifiData data;     

      EEPROM.get(0, data);

      if (!data.hasKeys) {
        TryPair();
      } else {
        int tries = 0;
        WiFi.begin(data.ssid, data.key);
        while (WiFi.status() != WL_CONNECTED) {
          tries++;
          delay(500);
          if (tries > 5) {
            TryPair();
            break;
          }
        }
      }
    }

  private:
    void TryPair() {
      WiFi.begin(WIFI_NAME_PAIR);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
      }

      String bridgeIp = WiFi.gatewayIP().toString();
      String url = "http://" + bridgeIp + "/keys";

      HTTPClient http;
      http.begin(url);

      int code = http.GET();
      if (code > 0) {
        String payload = http.getString();
        http.end();

        StaticJsonDocument<JSON_OBJECT_SIZE(2)> doc;
        DeserializationError err = deserializeJson(doc, payload);
        if (err != DeserializationError::Ok) {
          return;
        }
        JsonObject obj = doc.as<JsonObject>();

        String key = obj["key"];
        String token = obj["token"];

        WifiData data;
        data.hasKeys = true;
        data.ssid = WIFI_NAME_CTRL,
        data.key = key;
        data.token = token;

        EEPROM.put(0, data);
        EEPROM.commit();

        Connect();
        Register(data.token);
      }
    }

    void Register(String token) {
      StaticJsonDocument<JSON_OBJECT_SIZE(2)> doc;
      doc["token"] = token;
      doc["type"] = "device";
      String payload = doc.as<String>();

      String bridgeIp = WiFi.gatewayIP().toString();
      String url = "http://" + bridgeIp + "/register";


      HTTPClient http;
      http.begin(url);
      http.addHeader("Content-Type", "application/json");

      http.POST(payload);
      http.end();
    }


};
