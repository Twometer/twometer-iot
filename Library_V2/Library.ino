#include "TwometerIoT.h"

TwometerIoT iot;

void setup() {
    iot.describe("Smart LED Stripe", "GlowTec Industries", "A smart, fancy and colorful LED stripe light", TYPE_LIGHT);

    iot.begin();
}

void loop() {
    iot.update();
}