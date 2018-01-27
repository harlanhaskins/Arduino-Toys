#define MIC_THRESHOLD 600

AnalogInputPin mic(0);
DigitalOutputPin buzzer(2);

void setup() {
}

void loop() {
  int micVal = mic.read();
  buzzer.highIf(micVal <= 500);
}