#include <TwometerIoT.h>

// Should be unique and random
const String DEVICE_ID = "9fc60c97-0c0b-43a3-a556-18bd3faf453e";

TwometerIoT iot;

void setup() {
  // First the id, the name, then the type and finally the manufacturer
  iot.describe({DEVICE_ID, "LED Stripe", TYPE_LIGHTS_STRIPE, "Twometer Electronics"});

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
