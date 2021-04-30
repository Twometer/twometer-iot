#ifndef _DEVICE_DESCRIPTOR_H
#define _DEVICE_DESCRIPTOR_H

#include <Arduino.h>

struct DeviceDescriptor {
    String deviceId;
    String type;
    String modelName;
    String manufacturer
    String description;
};

#endif