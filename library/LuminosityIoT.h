#ifndef _LUMINOSITY_IOT_H
#define _LUMINOSITY_IOT_H

#include <Arduino.h>

#include "DeviceTypes.h"
#include "MessageTypes.h"
#include "PropertyTypes.h"
#include "DeviceDescriptor.h"

class LuminosityIoT
{
private:
public:
    void describe();
    void expose();
    void on();

    void begin();
    void update();
    void report();
};

#endif
