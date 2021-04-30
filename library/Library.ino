#include "LuminosityIoT.h"

LuminosityIoT iot;

void setup()
{
    /* Set up device properties */
    iot.setupWifi("uwunet", "uwunet pair");
    iot.describe("IoT Test Device", "GlowTec Industries", "A test device that exposes all types of properties and raises the test event every 10 seconds.", DEVICE_TYPE_LIGHT);
    iot.expose("Device.PowerState", PROPERTY_TYPE_BOOLEAN);
    iot.expose("Light.Brightness", PROPERTY_TYPE_NUMBER);
    iot.expose("Light.Color", PROPERTY_TYPE_COLOR);
    iot.expose("Button.Clicked", PROPERTY_TYPE_EVENT);
    iot.expose("TV.InputChannel", PROPERTY_TYPE_MODE)
        .setFriendlyName("Input channel")
        .addOption("HDMI1", "DVD player")
        .addOption("HDMI2", "Sat receiver");

    /* Set handlers */
    iot.on("Device.PowerState", [](Message &message) {
        bool powerState = message.readBool();
        Serial.printf("Set power state to: %d\n", powerState);
    });

    iot.on("Light.Brightness", [](Message &message) {
        int brightness = message.readInt();
        Serial.printf("Set light brightness to %d\n", brightness);
    });

    iot.on("Light.Color", [](Message &message) {
        Color color = message.readColor();
        Serial.printf("Set light color to [h=%d, s=%d, b=%d]\n", color.h, color.s, color.b);
    });

    iot.on("TV.InputChannel", [](Message &message) {
        String mode = message.readString();
        Serial.printf("Set input channel to [%s]\n", mode.c_str());
    });

    /* Initialize */
    iot.begin();
}

unsigned long lastReport = 0;

void loop()
{
    iot.update();

    if (millis() - lastReport > 10000)
    {
        lastReport = millis();

        /* Report a dummy value */
        Message message{};
        message.writeInt(42);
        iot.report("Button.Clicked", message);
    }
}