#ifndef _WIFI_CONTROLLER_H
#define _WIFI_CONTROLLER_H

#include <Arduino.h>
#include <ESP8266WiFi.h>

#include "HTTP.h"
#include "StorageBuf.h"

#define EEPROM_MAGIC 0xE2
#define DEBUG_MODE 0

class WiFiController
{
private:
    String wifiPassword;
    String authToken;
    String bridgeIp;

public:
    void connect(const String &controlSsid, const String &pairingSsid, const String &deviceDescription)
    {
        WiFi.persistent(false);
        WiFi.mode(WIFI_STA);
        EEPROM.begin(512);

        bool hasData = loadStorage();
        if (hasData)
        {
            Serial.println("EEPROM has data, connecting to WiFi...");
#if DEBUG_MODE
            Serial.setDebugOutput(true);
            WiFi.printDiag(Serial);
#endif
            WiFi.begin(controlSsid, wifiPassword);
            awaitConnection();
        }
        else
        {
            Serial.println("EEPROM has no data, searching for pairing bridge...");
            WiFi.begin(pairingSsid);
            awaitConnection();

            Serial.println("Found bridge! Pairing...");
            pair(deviceDescription);

            Serial.println("Successfully paired with the bridge, rebooting...");
            ESP.restart();
        }
    }

    const String &getAuthToken()
    {
        return authToken;
    }

private:
    void awaitConnection()
    {
        Serial.println("Connecting to " + WiFi.SSID() + "...");
        while (WiFi.status() != WL_CONNECTED)
        {
            delay(100);
        }
        bridgeIp = WiFi.gatewayIP().toString();
    }

    void pair(const String &deviceDescription)
    {
        String pairingUrl = "http://" + bridgeIp + "/pair";

        HTTPResponse response;

        while (true)
        {
            response = HTTP::Post(pairingUrl, deviceDescription);
            if (response.success && response.statusCode == 200)
                break;
            else
            {
                Serial.printf("Pairing failed (status code %d)\n", response.statusCode);
                delay(1000);
            }
        }

        StaticJsonDocument<192> doc;
        deserializeJson(doc, response.body);

        const char *wifiPassword = doc["wifiKey"];
        const char *authToken = doc["authToken"];
        this->wifiPassword = String(wifiPassword);
        this->authToken = String(authToken);
        saveStorage();
    }

    bool loadStorage()
    {
        StorageBuf buf;
        if (buf.readByte() != EEPROM_MAGIC)
            return false;
        wifiPassword = buf.readString();
        authToken = buf.readString();
        return true;
    }

    void saveStorage()
    {
        StorageBuf buf;
        buf.writeByte(EEPROM_MAGIC);
        buf.writeString(wifiPassword);
        buf.writeString(authToken);
        buf.save();
    }

    void clearStorage()
    {
        EEPROM.write(0, 0);
        EEPROM.commit();
    }
};

#endif