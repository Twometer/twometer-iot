#ifndef _PROPERTY_H
#define _PROPERTY_H

#include <Arduino.h>

struct ModeOption
{
    String key;
    String friendlyName;
};

struct Property
{
    String name;
    String type;

    String friendlyName;
    std::vector<ModeOption> modeOptions;

    Property(const String &name, const String &type)
        : name(name), type(type)
    {
    }

    Property &setFriendlyName(const String &friendlyName)
    {
        this->friendlyName = friendlyName;
        return *this;
    }

    Property &addOption(const String &key, const String &friendlyName)
    {
        modeOptions.push_back({key, friendlyName});
        return *this;
    }
};

#endif