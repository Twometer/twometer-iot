#include "StorageBuf.h"

/**
   Describes a registered device
*/
struct DeviceDescriptor {
  String deviceId;
  String modelName;
  String manufacturer;
  String description;
  String type;
  String friendlyName;

  void write(StorageBuf& buf) {
    buf.write_string(deviceId);
    buf.write_string(modelName);
    buf.write_string(manufacturer);
    buf.write_string(description);
    buf.write_string(type);
    buf.write_string(friendlyName);
  }

  void read(StorageBuf& buf) {
    deviceId = buf.read_string();
    modelName = buf.read_string();
    manufacturer = buf.read_string();
    description = buf.read_string();
    type = buf.read_string();
    friendlyName = buf.read_string();
  }

  bool operator==(const DeviceDescriptor& b) {
    return this->deviceId == b.deviceId;
  }
};

/**
   Describes a connected device
*/
struct ConnectedDevice {
  String uuid;
  String ip;
  int missedHeartbeats;

  bool operator==(const ConnectedDevice& b) {
    return this->uuid == b.uuid;
  }
};

/**
   Parses a device descriptor from JSON
*/
DeviceDescriptor parseDeviceDescriptor(String str) {
  StaticJsonDocument<JSON_OBJECT_SIZE(4)> doc;
  deserializeJson(doc, str);

  String deviceId = doc["deviceId"];
  String modelName = doc["modelName"];
  String manufacturer = doc["manufacturer"];
  String description = doc["description"];
  String type = doc["type"];

  return { deviceId, modelName, manufacturer, description, type, "" };
}

DeviceDescriptor parseDeviceDescriptor(DynamicJsonDocument doc) {
  String deviceId = doc["deviceId"];
  String modelName = doc["modelName"];
  String manufacturer = doc["manufacturer"];
  String description = doc["description"];
  String type = doc["type"];

  return { deviceId, modelName, manufacturer, description, type, "" };
}
