#include <TwometerIoT.h>

TwometerIoT iot;

void setup() {
  // First the name, then the type and finally the manufacturer
  iot.describe({"LED Stripe", TYPE_LIGHTS_STRIPE, "Twometer Electronics"});

  iot.prop("color", DATA_COLOR_RGB)
  .handle([](const Request & req) {
    ColorRgb rgb = req.colorRgbVal();

    // TODO Update rgb outputs

    return rgb.b == 42;
  });

  iot.begin();
}

void loop() {
  iot.update();
}
