#include "StorageBuf.h"

/**
   Describes a registered device
*/
struct DeviceDescriptor {
  String uuid;
  String name;
  String type;
  String manufacturer;
  String role;
  String friendlyName;

  void write(StorageBuf& buf) {
    buf.write_string(uuid);
    buf.write_string(name);
    buf.write_string(type);
    buf.write_string(manufacturer);
    buf.write_string(role);
  }

  void read(StorageBuf& buf) {
    uuid = buf.read_string();
    name = buf.read_string();
    type = buf.read_string();
    manufacturer = buf.read_string();
    role = buf.read_string();
  }

  bool operator==(const DeviceDescriptor& b) {
    return this->uuid == b.uuid;
  }
};

/**
   Describes a connected device
*/
struct ConnectedDevice {
  String uuid;
  String ip;

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

  String uuid = doc["uuid"];
  String name = doc["name"];
  String type = doc["type"];
  String manufacturer = doc["manufacturer"];

  return { uuid, name, type, manufacturer, "", "" };
}

DeviceDescriptor parseDeviceDescriptor(DynamicJsonDocument doc) {
  String uuid = doc["uuid"];
  String name = doc["name"];
  String type = doc["type"];
  String manufacturer = doc["manufacturer"];

  return { uuid, name, type, manufacturer, "", "" };
}
