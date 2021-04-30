#ifndef _PROPERTY_H
#define _PROPERTY_H

#include <Arduino.h>

struct ModeOption
{
    String key;
    String friendlyName;
};

class Property
{
private:
    String name;
    String type;

    String friendlyName;
    std::vector<ModeOption> modeOptions;

public:
    Property(const String &name, const String &type)
        : name(name), type(type)
    {
    }

    const Property &setFriendlyName(const String &friendlyName)
    {
        this->friendlyName = friendlyName;
        return *this;
    }

    const Property &addOption(const String &key, const String &friendlyName)
    {
        modeOptions.push_back({key, friendlyName});
        return *this;
    }
};

#endif