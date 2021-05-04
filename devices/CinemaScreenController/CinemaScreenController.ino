#include <LuminosityIoT.h>

LuminosityIoT iot;

#define PIN_OPEN 14
#define PIN_STOP 12
#define PIN_CLOSE 13

void hwInit()
{
    pinMode(PIN_OPEN, FUNCTION_3);
    pinMode(PIN_OPEN, OUTPUT);
    pinMode(PIN_STOP, FUNCTION_3);
    pinMode(PIN_STOP, OUTPUT);
    pinMode(PIN_CLOSE, FUNCTION_3);
    pinMode(PIN_CLOSE, OUTPUT);

    digitalWrite(PIN_OPEN, LOW);
    digitalWrite(PIN_STOP, LOW);
    digitalWrite(PIN_CLOSE, LOW);
}

void setup()
{
    hwInit();

    iot.configure("Twometer IoT", "Twometer IoT Pair", 11210);
    iot.describe("Cinema Screen", "Twometer Electronics", "Screen controller for the Twometer Cinema system", DEVICE_TYPE_INTERIOR_BLIND);
    iot.expose("Blinds.Position", PROPERTY_TYPE_MODE)
        .setFriendlyName("Position")
        .addOption("Position.Up", "Hoch")
        .addOption("Position.Down", "Runter");

    iot.on("Blinds.Position", [](const Message &message) {
        String position = message.readString();
        if (position == "Position.Up")
        {
            digitalWrite(PIN_OPEN, HIGH);
            delay(500);
            digitalWrite(PIN_OPEN, LOW);
        }
        else if (position == "Position.Down")
        {
            digitalWrite(PIN_CLOSE, HIGH);
            delay(500);
            digitalWrite(PIN_CLOSE, LOW);
        }
    });

    iot.begin();
}

void loop()
{
    iot.update();
}
