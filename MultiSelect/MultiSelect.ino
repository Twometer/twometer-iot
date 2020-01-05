#include <TwometerIoT.h>

// Should be unique and random
const String DEVICE_ID = "e5c096a2-30f1-4f39-813f-22f52af82aa1";

TwometerIoT iot;

void setup() {
  // First the id, the name, then the type and finally the manufacturer
  iot.describe({DEVICE_ID, "Multi Button Selector", TYPE_BUTTON_MULTI, "Twometer Electronics"});

  iot.prop("output", DATA_INT)
  .handle([](const Request & req) {
    int out = req.intVal();

    if (out == 0) {
      digitalWrite(2, HIGH);
      delay(500);
      digitalWrite(2, LOW);
    } else if (out == 1) {
      digitalWrite(3, HIGH);
      delay(500);
      digitalWrite(3, LOW)
    } else return false;

    return true;
  });

  iot.begin();
}

void loop() {
  iot.update();
}
