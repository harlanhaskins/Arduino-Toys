#include "ArduinoUtils.h"

#define MIC_THRESHOLD 600

class Clapper {
private:
  enum State {
    Idle,
    OneClap
  };
  DigitalOutputPin &pin;
  State state;
  bool isOn;
  long cooldownMillis;
  long resetMillis;
public:
  Clapper(DigitalOutputPin &pin): 
    pin(pin), state(Idle), isOn(true), cooldownMillis(0) {}

  void setState(State state) {
    cooldownMillis = millis() + 100;
    Serial.print("State is now: ");
    switch (state) {
    case Idle:
      Serial.println("Idle");
      resetMillis = 0;
      break;
    case OneClap:
      Serial.println("OneClap");
      resetMillis = millis() + 500;
      break;
    }
    this->state = state;
  }

  bool isCoolingDown() {
    return millis() < cooldownMillis;
  }

  void update(int micValue) {
    if (isCoolingDown()) return;
    switch (state) {
    case Idle:
      if (micValue < MIC_THRESHOLD) return;
      setState(OneClap);
      break;
    case OneClap:
      if (millis() <= resetMillis) {
        if (micValue >= MIC_THRESHOLD) {
          setState(Idle);
          Serial.println("Setting LED pin!");
          delay(50);
          pin.highIf(isOn);
          isOn = !isOn;
        }
      } else {
        setState(Idle);
      }
      break;
    }
  }
};

AnalogInputPin mic(0);
DigitalOutputPin led(2);
Clapper clapper(led);

void setup() {
}

void loop() {
  clapper.update(mic.read());
}