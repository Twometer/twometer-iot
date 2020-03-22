#include <TwometerIoT.h>
#include "HSBColor.h"

TwometerIoT iot;

#define PIN_R 0
#define PIN_G 1
#define PIN_B 2
#define PIN_W 3

struct Color {
  float h;
  float s;
  float b;
};

bool hasChanged = true;
bool currentPowerState = false;
Color currentSetting { 0.0f, 100.0f, 100.0f };

Color thisColor { 0.0f, 100.0f, 0.0f };
Color lastColor { 0.0f, 100.0f, 0.0f };

void setup() {
  iot.describe("Smart LED Stripe", "Twometer Electronics", "A smart, fancy and colorful LED stripe light", TYPE_LIGHT);

  iot.handle("Lamp.Brightness", [](const DynamicJsonDocument & payload) {
    int brightness = payload["brightness"].as<float>() * 100.0f;
    currentSetting.b = (float)brightness;
    hasChanged = true;
    return true;
  });

  iot.handle("Lamp.Color", [](const DynamicJsonDocument & payload) {
    currentSetting.h = payload["hue"];
    currentSetting.s = payload["saturation"].as<float>() * 100.0f;
    hasChanged = true;
    return true;
  });

  iot.handle("Device.PowerState", [](const DynamicJsonDocument & payload) {
    currentPowerState = payload["powerState"].as<bool>();
    hasChanged = true;
    return true;
  });

  iot.begin();

  pinMode(PIN_R, FUNCTION_3);
  pinMode(PIN_G, FUNCTION_3);
  pinMode(PIN_B, FUNCTION_3);
  pinMode(PIN_W, FUNCTION_3);

  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  pinMode(PIN_W, OUTPUT);
}

void loop() {
  iot.update();

  if (hasChanged) {
    thisColor.h = currentSetting.h;
    thisColor.s = currentSetting.s;
    thisColor.b = (currentPowerState ? currentSetting.b : 0.0f);

    float time_step = 35; // ms
    for (float i = 0.0f; i <= 1.0f; i += 0.1f) {
      Color color = interpolate(lastColor, thisColor, i);
      writePwm(color);
      delay(time_step);
    }

    writePwm(thisColor);

    lastColor = { thisColor.h, thisColor.s, thisColor.b };
    hasChanged = false;
  }
}

void writePwm(Color color) { 
  int rgb[3];
  H2R_HSBtoRGB(color.h, color.s, color.b, rgb);

  if (color.s > 20) {
    analogWrite(PIN_R, remapToPwm(rgb[0]));
    analogWrite(PIN_G, remapToPwm(rgb[1]));
    analogWrite(PIN_B, remapToPwm(rgb[2]));
    analogWrite(PIN_W, 0);
  } else {
    analogWrite(PIN_R, 0);
    analogWrite(PIN_G, 0);
    analogWrite(PIN_B, 0);
    analogWrite(PIN_W, remapToPwm(int(color.b * 2.55f)));
  }
}

Color interpolate(Color a, Color b, float v) {
  float ch = a.h + (b.h - a.h) * v;
  float cs = a.s + (b.s - a.s) * v;
  float cb = a.b + (b.b - a.b) * v;
  return { ch, cs, cb };
}

// RGB is 0...255 but PWM is 0...1023 on ESP8266
// Therefore, we have to remap this
int remapToPwm(int colorVal) {
  if (colorVal == 0)
    return 0;

  // 255 + 1 = 256; 256*4 = 1024; 1024-1 = 1023 => limits are OK
  return (colorVal + 1) * 4 - 1;
}
