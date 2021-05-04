#include <LuminosityIoT.h>

LuminosityIoT iot;

void hwInit()
{
    pinMode(2, OUTPUT);
    pinMode(3, FUNCTION_3);
    pinMode(3, OUTPUT);
    digitalWrite(2, LOW);
    digitalWrite(3, LOW);
}

void musicMode()
{
    digitalWrite(2, HIGH);
    delay(500);
    digitalWrite(2, LOW);
}

void cinemaMode()
{
    digitalWrite(3, HIGH);
    delay(500);
    digitalWrite(3, LOW);
}

void setup()
{
    hwInit();
    delay(2000);
    musicMode();

    iot.configure("Twometer IoT", "Twometer IoT Pair", 11210);
    iot.describe("Audio Amplifier", "Twometer Electronics", "Channel switcher attached to the Twometer Cinema sound system", DEVICE_TYPE_MUSIC_SYSTEM);
    iot.expose("Amp.InputChannel", PROPERTY_TYPE_MODE)
        .setFriendlyName("Kanal")
        .addOption("InputChannel.Music", "Musik")
        .addOption("InputChannel.Cinema", "Kino");

    iot.on("Amp.InputChannel", [](Message &message) {
        String channel = message.readString();
        if (channel == "InputChannel.Music")
            musicMode();
        else if (channel == "InputChannel.Cinema")
            cinemaMode();
    });
    iot.begin();

    musicMode();
}

void loop()
{
    iot.update();
}
