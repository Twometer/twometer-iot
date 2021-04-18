#ifndef property_h
#define property_h

#define PROPERTY_REGULAR 1
#define PROPERTY_MODE    2

class Property {
public:
    int propertyType;

    String name;

    std::function<bool(const DynamicJsonDocument&)> handler;

    DynamicJsonDocument *currentState;

    virtual ~Property() = default;

};

#endif