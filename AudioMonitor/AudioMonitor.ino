#include "ArduinoUtils.h"

AnalogInputPin mic(0);
DigitalOutputPin led1(2);
DigitalOutputPin led2(4);
DigitalOutputPin led3(7);

void setup() {
}

void loop() {
  int micLevel = mic.read();
  led1.highIf(micLevel >= 530);
  led2.highIf(micLevel >= 540);
  led3.highIf(micLevel >= 550);
}