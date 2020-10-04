#include <TwometerIoT.h>
#include "HSBColor.h"

TwometerIoT iot;

#define PIN_R 5
#define PIN_G 4
#define PIN_B 14
#define PIN_W 12

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
    int brightness = payload["brightness"].as<float>();
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

    for (float i = 0.0f; i <= 1.0f; i += 0.01f) {
      Color color = interpolate(lastColor, thisColor, i);
      writePwm(color);
      delay(4);
    }

    writePwm(thisColor);

    lastColor = { thisColor.h, thisColor.s, thisColor.b };
    hasChanged = false;
  }
}

void writePwm(Color color) {
  int rgb[3];
  H2R_HSBtoRGB(color.h, color.s, color.b, rgb);

  if (color.s > 20) { // Above 20, use RGB LEDs
    analogWrite(PIN_R, remapToPwm(rgb[0]));
    analogWrite(PIN_G, remapToPwm(rgb[1]));
    analogWrite(PIN_B, remapToPwm(rgb[2]));
    analogWrite(PIN_W, 0);
  } else if (color.s >= 1) { // From 1..20, use RGB and W LEDs
    float rgbFac = color.s / 100.0f;
    float wFac = 1.0 - rgbFac;
    analogWrite(PIN_R, remapToPwm(rgb[0] * rgbFac));
    analogWrite(PIN_G, remapToPwm(rgb[1] * rgbFac));
    analogWrite(PIN_B, remapToPwm(rgb[2] * rgbFac));
    analogWrite(PIN_W, remapToPwm(int(color.b * 2.55f * wFac)));
  } else { // Saturation ~0 just set it to white
    analogWrite(PIN_R, 0);
    analogWrite(PIN_G, 0);
    analogWrite(PIN_B, 0);
    analogWrite(PIN_W, remapToPwm(int(color.b * 2.550f)));
  }
}

Color interpolate(Color a, Color b, float v) {
  float ch = interpolateHue(a.h, b.h, v);
  float cs = a.s + (b.s - a.s) * v;
  float cb = a.b + (b.b - a.b) * v;
  return { ch, cs, cb };
}

float interpolateHue(float h1, float h2, float v) {
  float d = h2 - h1;
  float delta = (d + ((abs(d) > 180.f) ? ((d < 0) ? 360.f : -360.f) : 0.f));
  return int((h1 + (delta * v)) + 360) % 360;
}

// RGB is 0...255 but PWM is 0...1023 on ESP8266
// Therefore, we have to remap this
int remapToPwm(int colorVal) {
  if (colorVal == 0)
    return 0;

  // 255 + 1 = 256; 256*4 = 1024; 1024-1 = 1023 => limits are OK
  return (colorVal + 1) * 4 - 1;
}
