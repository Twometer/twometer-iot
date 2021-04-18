#ifndef modeproperty_h
#define modeproperty_h

#include "Property.h"

struct ModeEntry {
    String key;

    String value;
};

class ModeProperty 
    : public Property {
public:

    String friendlyName;

    std::vector<ModeEntry> entries;

    ModeProperty &setFriendlyName(String friendlyName) {
        this->friendlyName = friendlyName;
        return *this;
    }

    ModeProperty &addMode(String key, String value) {
        entries.push_back({ key, value });
        return *this;
    }


};

#endif