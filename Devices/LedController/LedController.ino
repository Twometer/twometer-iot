#include <TwometerIoT.h>
#include "HSBColor.h"

TwometerIoT iot;

bool hasChanged = true;
bool currentPowerState = false;
float currentBrightness = 1.0f;
float currentH = 0.0f;
float currentS = 1.0f;

void setup() {
  iot.describe("Smart LED Stripe", "Twometer Electronics", "A smart, fancy and colorful LED stripe light", TYPE_LIGHT);

  iot.handle("Lamp.Brightness", [](const DynamicJsonDocument & payload) {
    int brightness = payload["brightness"];
    currentBrightness = (float)brightness / 100.0f;
    hasChanged = true;
    return true;
  });

  iot.handle("Lamp.Color", [](const DynamicJsonDocument & payload) {
    currentH = payload["hue"];
    currentS = payload["saturation"];
    hasChanged = true;
    return true;
  });

  iot.handle("Device.PowerState", [](const DynamicJsonDocument & payload) {
    currentPowerState = payload["powerState"].as<bool>();
    hasChanged = true;
    return true;
  });

  iot.begin();
}

void loop() {
  iot.update();

  if (hasChanged) {
    float h = currentH;
    float s = curentS * 100.f;
    float b = (currentPowerState ? currentBrightness : 0.0f) * 100.f;
    int rgb[3];
    H2R_HSBtoRGB(h, s, b, rgb);

    // TODO Update RGB pins accordingly
    hasChanged = false;
  }
}
