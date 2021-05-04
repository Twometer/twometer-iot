#include <TwometerIoT.h>

TwometerIoT iot;

#define PIN_PWR 14

int curPwm = 1023;

void setup() {
  iot.describe("Desk LEDs", "Twometer Electronics", "Desk underglow LEDs", DEVICE_TYPE_LIGHT);

  iot.handle("Device.PowerState", [](const DynamicJsonDocument & payload) {
    int nextPwm = payload["powerState"].as<bool>() ? 1023 : 0;

    for (float i = 0.0f; i <= 1.0f; i += 0.01f) {
      int pwm = int(curPwm + float(nextPwm - curPwm) * i);
      analogWrite(PIN_PWR, pwm);
      delay(4);
    }
    curPwm = nextPwm;
    return true;
  });

  iot.begin();

  pinMode(PIN_PWR, FUNCTION_3);
  pinMode(PIN_PWR, OUTPUT);
  analogWrite(PIN_PWR, curPwm);
}

void loop() {
  iot.update();
}
