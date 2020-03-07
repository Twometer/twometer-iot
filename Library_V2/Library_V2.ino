#include "TwometerIoT.h"

TwometerIoT iot;

void setup() {
    iot.describe("Smart LED Stripe", "GlowTec Industries", "A smart, fancy and colorful LED stripe light", TYPE_LIGHT);

    iot.handle("Lamp.Brightness", [](const DynamicJsonDocument &payload) {
        // TODO
        int brightness = payload["brightness"];
        Serial.println("Set brightness to: " + String(brightness));

        return true;
    });

    iot.handle("Lamp.Color", [](const DynamicJsonDocument &payload) {
        float hue = payload["hue"];
        float saturation = payload["saturation"];
        float brightness = payload["brightness"];

        Serial.println("Set color to: H=" + String(hue) + " S=" + String(saturation) + " B=" + String(brightness));

        // TODO
        return true;
    });

    iot.handle("Device.PowerState", [](const DynamicJsonDocument &payload) {
        bool powerState = payload["powerState"];

        Serial.println("Set power state to: " + String(powerState));

        return true;
    });

    iot.handleMode("TV.InputChannel", [](const DynamicJsonDocument &payload) {
        String mode = payload["mode"];

        Serial.println("Set input channel to: " + mode);
        
        return true;
    })
    .setFriendlyName("Kanal")
    .addMode("InputChannel.Music", "Musik")
    .addMode("InputChannel.Cinema", "Heimkino");

    iot.begin();
}

void loop() {
    iot.update();
}
