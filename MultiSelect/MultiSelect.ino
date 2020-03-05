#include <TwometerIoT.h>

TwometerIoT iot;

void setup() {
  // First the name, then the type and finally the manufacturer
  iot.describe({"Multi Button Selector", TYPE_BUTTON_MULTI, "Twometer Electronics"});

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
