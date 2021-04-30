#ifndef _LUMINOSITY_IOT_H
#define _LUMINOSITY_IOT_H

#include <Arduino.h>
#include <ArduinoJson.h>

#include "Color.h"
#include "Message.h"
#include "Property.h"
#include "DeviceTypes.h"
#include "MessageTypes.h"
#include "PropertyTypes.h"
#include "WiFiController.h"
#include "DeviceDescriptor.h"

class LuminosityIoT
{
private:
    String controlSsid;
    String pairingSsid;

    WiFiController wiFiController;
    DeviceDescriptor deviceDescriptor;
    std::vector<Property *> properties;

public:
    void setupWifi(const String &controlSsid, const String &pairingSsid)
    {
        this->controlSsid = controlSsid;
        this->pairingSsid = pairingSsid;
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
    void on(const String &property, std::function<void(Message &)> handler)
    {
    }

    void begin()
    {
        Serial.begin(115200);
        Serial.println("Luminosity IoT System version 3.0.0");
        wiFiController.connect(controlSsid, pairingSsid, serializeDeviceDescription());
    }
    void update()
    {
    }
    void report(const String &property, const Message &value)
    {
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
};

#endif
