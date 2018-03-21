#include "ArduinoUtils.h"

char ascii[28] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char lowerAscii[28] = " abcdefghijklmnopqrstuvwxyz";
const char *morse[27] = {
  " ", ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",
  ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",
  "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."
};

const DigitalOutputPin led(2);

const char *toMorse(char c) {
  int i = 0;
  for (; i < 28; i++) {
    if (ascii[i] == c || lowerAscii[i] == c) {
      return morse[i];
    }
  }
  return morse[0];
}

void dot() {
  led.high();
  delay(150);
  led.low();
  delay(150);
}

void dash() {
  led.high();
  delay(450);
  led.low();
  delay(150);
}

void displayChar(char c) {
  const char *morse = toMorse(c);
  while (*morse) {
    switch (*morse) {
    case '.':
      dot();
      break;
    case '-':
      dash();
      break;
    default:
      delay(200);
      break;
    }
    morse++;
  }
}

void displayMorse(const char *str) {
  const char *c = str;
  while (*c) {
    displayChar(*c);
    delay(200);
    ++c;
  }
}

void setup() {
}

void loop() {
  displayMorse("Hello world");
  delay(2000);
}