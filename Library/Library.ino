#include "TwometerIoT.h"

const String DEVICE_ID = "55ef1248-6460-4a33-a54b-1faa0d929e7e";

TwometerIoT iot;

void setup() {
  iot.describe({DEVICE_ID, "GlowTec LED Stripe", TYPE_LIGHT_STRIPE, "GlowTec Industries"});

  iot.prop("color", DATA_COLOR_RGB)
  .handle([](const Request &req) {
    ColorRgb rgb = req.colorRgbVal();

    // TODO Update rgb outputs

    return rgb.b == 42;
  });

  iot.begin();
}

void loop() {
  iot.update();
}
