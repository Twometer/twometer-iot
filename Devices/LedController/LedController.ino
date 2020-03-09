#include <TwometerIoT.h>
#include "HSBColor.h"

TwometerIoT iot;

#define PIN_R 0
#define PIN_G 1
#define PIN_B 2
#define PIN_W 3

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

    if (s > 20) {
      analogWrite(PIN_R, remapToPwm(rgb[0]);
      analogWrite(PIN_G, remapToPwm(rgb[1]);
      analogWrite(PIN_B, remapToPwm(rgb[2]);
      analogWrite(PIN_W, 0);
    } else {
      analogWrite(PIN_R, 0);
      analogWrite(PIN_G, 0);
      analogWrite(PIN_B, 0);
      analogWrite(PIN_W, int(b * 2.5f));
    }

    hasChanged = false;
  }
}

// RGB is 0...255 but PWM is 0...1023 on ESP8266
// Therefore, we have to remap this
void remapToPwm(int colorVal) {
  // 255 + 1 = 256; 256*4 = 1024; 1024-1 = 1023 => limits are OK
  return (colorVal + 1) * 4 - 1;
}
