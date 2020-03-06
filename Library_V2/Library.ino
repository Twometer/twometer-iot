#include "TwometerIoT.h"
#include "Payload.h"

TwometerIoT iot;

void setup() {
    iot.describe("Smart LED Stripe", "GlowTec Industries", "A smart, fancy and colorful LED stripe light", TYPE_LIGHT);

    iot.handle("Lamp.Brightness", [](Payload& payload) {

    });

    iot.handle("Lamp.Color", [](Payload& payload) {

    });

    iot.handleMode("TV.InputChannel", [](Payload& payload) {

    })
    .setName("Kanal")
    .addMode("InputChannel.Music", "Musik")
    .addMode("InputChannel.Cinema", "Heimkino");

    iot.begin();
}

void loop() {
    iot.update();
}