#include <TwometerIoT.h>

TwometerIoT iot;

void setup() {
  iot.describe("Smart Audio Amplifier", "Twometer Electronics", "A controller attached to the Twometer Cinema sound system", TYPE_MUSIC_SYSTEM);

  iot.handleMode("Amp.InputChannel", [](const DynamicJsonDocument & payload) {
    String mode = payload["mode"];

    if (mode == "InputChannel.Music") {
      digitalWrite(2, HIGH);
      delay(500);
      digitalWrite(2, LOW);
    } else if (mode == "InputChannel.Kino") {
      digitalWrite(3, HIGH);
      delay(500);
      digitalWrite(3, LOW);
    } else return false;

    return true;
  })
  .setFriendlyName("Kanal")
  .addMode("InputChannel.Music", "Musik")
  .addMode("InputChannel.Cinema", "Kino");

  iot.begin();

  // Initialize the amp to music mode
  digitalWrite(2, HIGH);
  delay(500);
  digitalWrite(2, LOW);
}

void loop() {
  iot.update();
}
