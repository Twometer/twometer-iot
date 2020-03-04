#ifndef storage_h
#define storage_h

#define EEPROM_INIT_MAGIC 42    // Magic header value to detect with a single byte read if any settings are present

#include "StorageBuf.h"
#include <EEPROM.h>

struct StorageData {
  std::vector<String> registrationTokens;
  std::vector<DeviceDescriptor> devices;
  std::vector<ConnectedDevice> connectedDevices;

  String wifiKey = "";
};

static StorageData STORAGE;

void storage_erase() {
  for (int i = 0; i < 4096; i++)
    EEPROM.write(i, 255);
  EEPROM.commit();
}

void storage_read() {
  EEPROM.begin(4096);
  StorageBuf buf;
  bool eeprom_has_data = buf.read_byte() == EEPROM_INIT_MAGIC;

  if (eeprom_has_data) { // If it does not have data, then don't read
    int regTokens = buf.read_int();
    for (int i = 0; i < regTokens; i++)
      STORAGE.registrationTokens.push_back(buf.read_string());

    int devices = buf.read_int();
    for (int i = 0; i < devices; i++) {
      DeviceDescriptor desc;
      desc.read(buf);
      STORAGE.devices.push_back(desc);
    }

    STORAGE.wifiKey = buf.read_string();

    Serial.println("Loaded settings: ");
    Serial.println(" Key: " + STORAGE.wifiKey);
    Serial.println(" Reg: " + String(STORAGE.registrationTokens.size()));
    Serial.println(" Dev: " + String(STORAGE.devices.size()));
    Serial.println(" Con: " + String(STORAGE.connectedDevices.size()));
  } else {
    Serial.println("Clean boot, no settings loaded");
  }
}

void storage_write() {
  StorageBuf buf;
  buf.write_byte(EEPROM_INIT_MAGIC);

  buf.write_int(STORAGE.registrationTokens.size());
  for (String& str : STORAGE.registrationTokens)
    buf.write_string(str);

  buf.write_int(STORAGE.devices.size());
  for (DeviceDescriptor& desc : STORAGE.devices)
    desc.write(buf);

  buf.write_string(STORAGE.wifiKey);

  EEPROM.commit();
}

#endif
