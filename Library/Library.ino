#include "TwometerIoT.h"

TwometerIoT iot;

void setup() {  
  iot.describe({"LED Stripe", TYPE_LIGHT_STRIPE, "Twometer Industries"});

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
