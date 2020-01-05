struct DeviceDescriptor {
  String uuid;
  String name;
  String type;
  String manufacturer;
  String role;

  bool operator==(const DeviceDescriptor& b) {
    return this->uuid == b.uuid;
  }
};

struct ConnectedDevice {
  String uuid;
  String ip;

  bool operator==(const ConnectedDevice& b) {
    return this->uuid == b.uuid;
  }
};

DeviceDescriptor parseDeviceDescriptor(String str) {
  StaticJsonDocument<JSON_OBJECT_SIZE(4)> doc;
  deserializeJson(doc, str);

  String uuid = doc["uuid"];
  String name = doc["name"];
  String type = doc["type"];
  String manufacturer = doc["manufacturer"];

  return { uuid, name, type, manufacturer };
}
