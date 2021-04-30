#ifndef _WIFI_CONTROLLER_H
#define _WIFI_CONTROLLER_H

#include <Arduino.h>

#include "HTTP.h"
#include "StorageBuf.h"

#define EEPROM_MAGIC 0xE2

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
        HTTP::Post(pairingUrl, deviceDescription);
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