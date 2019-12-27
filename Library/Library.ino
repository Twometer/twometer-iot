#include "TwometerIoT.h"

TwometerIoT iot;

void setup() {
  iot.describe({"GlowTec LED Strip", TYPE_LIGHT_STRIPE, "GlowTec Industries"});

  iot.prop("color")
  .intVal("r", 0, 255)
  .intVal("g", 0, 255)
  .intVal("b", 0, 255)
  .handle([](const Request & req) {
   // int r = req.intVal("r");
   // int g = req.intVal("g");
  // int b = req.intVal("b");

    // TODO Update rgb outputs
  });

  iot.begin();
}

void loop() {
  iot.update();
}
