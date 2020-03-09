#include <TwometerIoT.h>

TwometerIoT iot;

void setup() {
  pinMode(2, OUTPUT);
  delay(2000);
  digitalWrite(2, HIGH);
  delay(500);
  digitalWrite(2, LOW);
  
  iot.describe("Smart Audio Amplifier", "Twometer Electronics", "A controller attached to the Twometer Cinema sound system", TYPE_MUSIC_SYSTEM);

  iot.handleMode("Amp.InputChannel", [](const DynamicJsonDocument & payload) {
    String mode = payload["mode"];

    if (mode == "InputChannel.Music") {
      digitalWrite(2, HIGH);
      delay(500);
      digitalWrite(2, LOW);
    } else if (mode == "InputChannel.Cinema") {
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

  pinMode(2, OUTPUT);
  pinMode(3, FUNCTION_3);
  pinMode(3, OUTPUT);
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);

  // Initialize the amp to music mode
  digitalWrite(2, HIGH);
  delay(500);
  digitalWrite(2, LOW);
}

void loop() {
  iot.update();
}
