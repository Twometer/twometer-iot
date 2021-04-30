#ifndef _LUMINOSITY_IOT_H
#define _LUMINOSITY_IOT_H

#include <Arduino.h>

#include "Property.h"
#include "DeviceTypes.h"
#include "MessageTypes.h"
#include "PropertyTypes.h"
#include "DeviceDescriptor.h"

class LuminosityIoT
{
private:
    String controlSsid;
    String pairingSsid;

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
    void on(const String &property, std::function<void(const Message &)> handler)
    {
    }

    void begin()
    {
    }
    void update()
    {
    }
    void report(const String &property, const Message &value)
    {
    }
};

#endif
