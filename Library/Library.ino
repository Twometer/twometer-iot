#include "TwometerIoT.h"

TwometerIoT iot;

void setup() {
  iot.describe({"GlowTec LED Strip", TYPE_LIGHT_STRIPE, "GlowTec Industries"});

  iot.prop("color")
  .handle([](const Request & req) {
    int r = req.intVal("r", 0, 255);
    int g = req.intVal("g", 0, 255);
    int b = req.intVal("b", 0, 255);

    // TODO Update rgb outputs

    return b == 42;
  });

  iot.prop("state")
  .handle([](const Request & req) {
    String val = req.enumVal("value", { "cinema", "bluetooth" });

    // TODO Update switch outputs

    return b == "cinema";
  });

  iot.begin();
}

void loop() {
  iot.update();
}
