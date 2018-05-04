/*
fht_adc.pde
guest openmusiclabs.com 9.5.12
example sketch for testing the fht library.
it takes in data on ADC0 (Analog0) and processes them
with the fht. the data is sent out over the serial
port at 115.2kb.  there is a pure data patch for
visualizing the data.
*/

#include "ArduinoUtils.h"

DigitalOutputPin speaker(3);
DigitalInputPin button(2);
AnalogInputPin microphone(A0);
AnalogInputPin potentiometer(A1);

#define BUFSIZE 256
char buffer[BUFSIZE];
uint16_t bufferIndex = 0;

void setup() {
  Serial.begin(115200);
}

void loop() {
  if (!button.isHigh()) return;

  char c = (char)map(/*potentiometer.read()*/1023, 1, 1023, 0, 255);

  Serial.write(&c, 1);
  while (button.isHigh()) {
    int micVal = microphone.read();
    c = (char)map(micVal, 1, 1023, 0, 255);
    buffer[bufferIndex] = c;
    ++bufferIndex;
    if (bufferIndex == BUFSIZE) {
      Serial.write(buffer, BUFSIZE);
      bufferIndex = 0;
    }
  }
  if (bufferIndex != BUFSIZE - 1) {
    Serial.write(&buffer[bufferIndex], BUFSIZE - bufferIndex);
  }
  c = 0;
  Serial.write(&c, 1);
}
