#include "TwometerIoT.h"

TwometerIoT iot;

void setup() {
  iot.describe({"GlowTec LED Stripe", TYPE_LIGHT_STRIPE, "GlowTec Industries"});

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
