#ifndef wificontroller_h
#define wificontroller_h

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "DeviceDescriptor.h"
#include "StorageLib.h"
#include "HttpLib.h"

#define EEPROM_INIT_MAGIC 42

class WiFiController {
    private:
        const char* WIFI_NAME_PAIR = "Twometer IoT Pair";   // The Pairing WiFi
        const char* WIFI_NAME_CTRL = "Twometer IoT";        // The Control WiFi

        String wifiKey;
        String registerToken;

        int lastWifiState = WL_CONNECTED;
        bool initComplete = false;

    public:
        void connect(DeviceDescriptor& desc) {
            WiFi.persistent(false);
            WiFi.mode(WIFI_STA);

            EEPROM.begin(256);
            bool hasData = loadStorage();

            Serial.println("Device id: " + desc.deviceId);

            if (hasData) {
                Serial.println("EEPROM has data, connecting...");
                WiFi.begin(WIFI_NAME_CTRL, wifiKey);

                awaitConnected();
                login();
            } else {
                Serial.println("EEPROM has no data, registering...");

                WiFi.begin(WIFI_NAME_PAIR);
                awaitConnected();

                getKeys();
                WiFi.begin(WIFI_NAME_CTRL, wifiKey);
                awaitConnected();

                delay(5000);

                registerDevice(desc);
                login();
            }

            Serial.println("WiFi setup complete");
            initComplete = true;
        }

        void update() {
            if (!initComplete) return;
            if (lastWifiState != WL_CONNECTED && WiFi.status() == WL_CONNECTED) {
                delay(1000);
                Serial.println("WiFi Connection regained, reporting to bridge...");
                login();
                Serial.println("Done");
            } else if (lastWifiState == WL_CONNECTED && WiFi.status() != WL_CONNECTED) {
                Serial.println("WiFi Connection lost");
            }
            lastWifiState = WiFi.status();
        }

    private:
        void login() {
            Serial.println("Login...");

            String bridgeIp = WiFi.gatewayIP().toString();
            String url = "http://" + bridgeIp + "/login";

            HttpResponse response;
            do {
                response = http_post(url, "{\"uuid\": \"" + String(ESP.getChipId(), HEX) + "\"}");
                if (response.code == 403) {
                    // 403 forbidden
                    // Registration has changed, clear data and reboot

                    clearStorage();
                    ESP.restart();
                    return;
                }
            } while (response.code != 200);
        }

        void getKeys() {
            Serial.println("Authenticating...");

            String bridgeIp = WiFi.gatewayIP().toString();
            String url = "http://" + bridgeIp + "/keys";

            HttpResponse response = http_get(url);
            if (response.code == 200) {
                StaticJsonDocument < JSON_OBJECT_SIZE(2) + 90 > doc;
                DeserializationError err = deserializeJson(doc, response.data);
                if (err != DeserializationError::Ok) {
                    return;
                }
                JsonObject obj = doc.as<JsonObject>();

                this->wifiKey = obj["key"].as<String>();
                this->registerToken = obj["token"].as<String>();

                // Serial.println("Wifi key: " + this->wifiKey);
                // Serial.println("Register token: " + this->registerToken);

                saveStorage();
            }
        }

        void registerDevice(DeviceDescriptor& desc) {
            Serial.println("Registering...");

            int strLen =  desc.deviceId.length() + 1
                        + desc.modelName.length() + 1
                        + desc.manufacturer.length() + 1 
                        + desc.description.length() + 1 
                        + desc.type.length() + 1 
                        + this->registerToken.length() + 1 
                        + 64;

            DynamicJsonDocument doc(JSON_OBJECT_SIZE(5) + strLen);
            doc["deviceId"] = desc.deviceId;
            doc["modelName"] = desc.modelName;
            doc["manufacturer"] = desc.manufacturer;
            doc["description"] = desc.description;
            doc["type"] = desc.type;
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

        /* WiFi util functions */
        void awaitConnected() {
            Serial.println("Connecting to " + WiFi.SSID() + " ...");
            while (WiFi.status() != WL_CONNECTED) {
                delay(500);
            }
        }

        /* Storage util functions */
        bool loadStorage() {
            StorageBuf buf;
            bool hasData = buf.read_byte() == EEPROM_INIT_MAGIC;
            if (!hasData)
                return false;

            this->wifiKey = buf.read_string();
            return true;
        }

        void saveStorage() {
            StorageBuf buf;
            buf.write_byte(EEPROM_INIT_MAGIC);
            buf.write_string(this->wifiKey);
            EEPROM.commit();
        }

        void clearStorage() {
            for (int i = 0; i < 256; i++)
                EEPROM.write(i, 255);
            EEPROM.commit();
        }
};

#endif