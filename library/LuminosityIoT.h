#ifndef _LUMINOSITY_IOT_H
#define _LUMINOSITY_IOT_H

#include <map>
#include <stdint.h>
#include <Arduino.h>
#include <ArduinoJson.h>

#include "Color.h"
#include "Config.h"
#include "Message.h"
#include "Property.h"
#include "DeviceTypes.h"
#include "MessageTypes.h"
#include "PropertyTypes.h"

#include "UdpClient.h"
#include "WiFiController.h"
#include "DeviceDescriptor.h"

#define RECVBUFSIZE 256

typedef std::function<void(Message &)> PropertyHandler;

class LuminosityIoT
{
private:
    int udpPort;
    String controlSsid;
    String pairingSsid;

    UdpClient udpClient;
    WiFiController wiFiController;
    DeviceDescriptor deviceDescriptor;
    std::vector<Property *> properties;
    std::map<String, PropertyHandler> propertyHandlers;

    uint8_t *receiveBuffer;
    unsigned long lastPing;

public:
    void configure(const String &controlSsid, const String &pairingSsid, int udpPort)
    {
        this->controlSsid = controlSsid;
        this->pairingSsid = pairingSsid;
        this->udpPort = udpPort;
    }
    void describe(const String &modelName, const String &manufacturer, const String &description, const String &type)
    {
        String deviceId = String(ESP.getChipId(), HEX);
        deviceDescriptor = {deviceId, type, modelName, manufacturer, description};
    }
    Property &expose(const String &propertyName, const String &propertyType)
    {
        auto property = new Property(propertyName, propertyType);
        properties.push_back(property);
        return *property;
    }
    void on(const String &property, PropertyHandler handler)
    {
        propertyHandlers[property] = handler;
    }

    void begin()
    {
        Serial.begin(115200);
        Serial.println("Luminosity IoT System version 3.0.0");
        wiFiController.connect(controlSsid, pairingSsid, serializeDeviceDescription());
        Serial.println("Connected to the WiFi");

        udpClient.begin(WiFi.gatewayIP(), udpPort);
        Serial.println("Connected to the UDP server");

        send(createMessage(MESSAGE_TYPE_DHELLO));
        Serial.println("Reported as online to the bridge.");

        lastPing = millis();
        receiveBuffer = new uint8_t[RECVBUFSIZE];
        memset(receiveBuffer, 0, RECVBUFSIZE);
    }
    void update()
    {
        wiFiController.checkConnection();
        checkPingTimeout();

        int len = udpClient.readPacket(receiveBuffer, RECVBUFSIZE);
        if (len <= 0)
            return;

        receiveBuffer[len] = 0x00;

        Message message;
        message.parse((const char *)receiveBuffer);

        if (message.getType() == MESSAGE_TYPE_PING)
        {
            lastPing = millis();
            send(createMessage(MESSAGE_TYPE_PONG));
        }
        else if (message.getType() == MESSAGE_TYPE_UPDATE)
        {
            auto property = message.readString();
            auto iterator = propertyHandlers.find(property);
            if (iterator != propertyHandlers.end())
            {
                iterator->second(message);
            }
            else
            {
                Serial.println("No handler for changed property " + property);
            }
        }
        else
        {
            Serial.println("Received unknown message type " + message.getType());
        }
    }
    void send(const Message &value)
    {
        String data = value.toString();
        udpClient.beginPacket();
        udpClient.writeRaw(data.c_str(), data.length());
        udpClient.endPacket();
    }

    Message createMessage(const String &type)
    {
        Message message(type);
        message.writeString(deviceDescriptor.deviceId);
        message.writeString(wiFiController.getAuthToken());
        return message;
    }

private:
    String serializeDeviceDescription()
    {
        DynamicJsonDocument doc(1536);
        doc["deviceId"] = deviceDescriptor.deviceId;
        doc["type"] = deviceDescriptor.type;
        doc["modelName"] = deviceDescriptor.modelName;
        doc["manufacturer"] = deviceDescriptor.manufacturer;
        doc["description"] = deviceDescriptor.description;

        JsonArray properties = doc.createNestedArray("properties");
        for (auto property : this->properties)
        {
            JsonObject propertyObj = properties.createNestedObject();
            propertyObj["name"] = property->name;
            propertyObj["type"] = property->type;
            propertyObj["friendlyName"] = property->friendlyName;
            if (property->type == PROPERTY_TYPE_MODE)
                propertyObj["valueRange"] = serializeModeOptions(property);
        }

        String serialized;
        serializeJson(doc, serialized);
        return serialized;
    }

    String serializeModeOptions(Property *property)
    {
        StaticJsonDocument<192> doc;
        for (auto modeOption : property->modeOptions)
        {
            doc[modeOption.key] = modeOption.friendlyName;
        }

        String serialized;
        serializeJson(doc, serialized);
        return serialized;
    }

    void checkPingTimeout()
    {
        if (millis() - lastPing > 15000)
        {
            lastPing = millis();
            send(createMessage(MESSAGE_TYPE_DHELLO));
        }
    }
};

#endif
