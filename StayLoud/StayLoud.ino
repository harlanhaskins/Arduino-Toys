#include "ArduinoUtils.h"

AnalogInputPin mic(0);
DigitalOutputPin buzzer(2);

void setup() {
}

void loop() {
  int micVal = mic.read();
  buzzer.highIf(micVal <= 800);
}
