#include <TwometerIoT.h>

TwometerIoT iot;

#define PIN_OPEN 14
#define PIN_STOP 12
#define PIN_CLOSE 13

void setup() {
  iot.describe("Cinema screen", "Twometer Electronics", "Remotely control the cinema screen", TYPE_INTERIOR_BLIND);

  iot.handleMode("Blinds.Position", [](const DynamicJsonDocument & payload) {
    String mode = payload["mode"];

    if (mode == "Position.Up") {
      digitalWrite(PIN_OPEN, HIGH);
      delay(500);
      digitalWrite(PIN_OPEN, LOW);
      Serial.println("Opening");
    } else if (mode == "Position.Down") {
      digitalWrite(PIN_CLOSE, HIGH);
      delay(500);
      digitalWrite(PIN_CLOSE, LOW);
      Serial.println("Closing");
    } else return false;

    return true;
  })
  .setFriendlyName("Position")
  .addMode("Position.Up", "Hoch")
  .addMode("Position.Down", "Runter");

  iot.begin();

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

void loop() {
  iot.update();
}
