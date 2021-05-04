#include <LuminosityIoT.h>
#include "ColorConvert.h"

LuminosityIoT iot;

#define PINOUT_VERSION 2

#if PINOUT_VERSION == 3     // PCB
#define PIN_R 13
#define PIN_G 12
#define PIN_B 14
#define PIN_W 4
#elif PINOUT_VERSION == 2   // D
#define PIN_R 5
#define PIN_G 4
#define PIN_B 14
#define PIN_W 12
#endif

bool hasChanged = true;
bool currentPowerState = false;
hsv currentSetting{0.0f, 1.0f, 1.0f};
hsv lastColor{0.0f, 1.0f, 0.0f};

void hwInit()
{
    pinMode(PIN_R, FUNCTION_3);
    pinMode(PIN_G, FUNCTION_3);
    pinMode(PIN_B, FUNCTION_3);
    pinMode(PIN_W, FUNCTION_3);

    pinMode(PIN_R, OUTPUT);
    pinMode(PIN_G, OUTPUT);
    pinMode(PIN_B, OUTPUT);
    pinMode(PIN_W, OUTPUT);
}

void setup()
{
    hwInit();

    iot.configure("Twometer IoT", "Twometer IoT Pair", 11210);
    iot.describe("LED Light", "Twometer Electronics", "Smart and colorful LED light", DEVICE_TYPE_LIGHT);
    iot.expose("Device.PowerState", PROPERTY_TYPE_BOOLEAN);
    iot.expose("Light.Color", PROPERTY_TYPE_COLOR);
    iot.expose("Light.ColorStream", PROPERTY_TYPE_COLOR);
    iot.expose("Light.Brightness", PROPERTY_TYPE_NUMBER);

    iot.on("Light.Brightness", [](Message &message) {
        currentSetting.v = message.readInt() / 100.0;
        hasChanged = true;
    });

    iot.on("Light.Color", [](Message &message) {
        currentSetting.h = (double)message.readInt();
        currentSetting.s = (double)message.readInt() / 255.0;
        hasChanged = true;
    });

    iot.on("Light.ColorStream", [](Message &message) {
        currentSetting.h = (double)message.readInt();
        currentSetting.s = (double)message.readInt() / 255.0;
        currentSetting.v = (double)message.readInt() / 255.0;
        writeColor(currentSetting);
    });

    iot.on("Device.PowerState", [](Message &message) {
        currentPowerState = message.readBool();
        hasChanged = true;
    });

    iot.begin();
}

void loop()
{
    iot.update();

    if (hasChanged)
    {
        hsv newColor = {currentSetting.h,
                        currentSetting.s,
                        (currentPowerState ? currentSetting.v : 0.0f)};

        // Fade animation to new color
        for (float i = 0.0f; i <= 1.0f; i += 0.01f)
        {
            hsv tmp = interpolate(lastColor, newColor, i);
            writeColor(tmp);
            delay(4);
        }

        writeColor(newColor);
        lastColor = {newColor.h, newColor.s, newColor.v};
        hasChanged = false;
    }
}

void writeColor(hsv hsv)
{
    rgb rgb = hsv2rgb(hsv);

    // Bias RGB and White LEDs based on saturation
    double rgbFactor = hsv.s > 0.1 ? 1.0 : (hsv.s / 0.1);
    double whiteFactor = 1.0 - rgbFactor;

    // Write PWM values
    analogWrite(PIN_R, rgb.r * 1024.0 * rgbFactor);
    analogWrite(PIN_G, rgb.g * 1024.0 * rgbFactor);
    analogWrite(PIN_B, rgb.b * 1024.0 * rgbFactor);
    analogWrite(PIN_W, hsv.v * 1024.0 * whiteFactor);
}

hsv interpolate(hsv a, hsv b, float k)
{
    double ch = interpolateHue(a.h, b.h, k);
    double cs = a.s + (b.s - a.s) * k;
    double cv = a.v + (b.v - a.v) * k;
    return {ch, cs, cv};
}

float interpolateHue(float h1, float h2, float k)
{
    float d = h2 - h1;
    float delta = (d + ((abs(d) > 180.f) ? ((d < 0) ? 360.f : -360.f) : 0.f));
    return int((h1 + (delta * k)) + 360) % 360;
}