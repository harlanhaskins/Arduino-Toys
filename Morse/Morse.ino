#include "ArduinoUtils.h"

char *ascii = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char *lowerAscii = "abcdefghijklmnopqrstuvwxyz";
const char *morse[26] = {
  ".-  ", "-...", "-.-.", "-.. ", ".   ", "..-.", "--. ", "....", "..  ",
  ".---", "-.- ", ".-..", "--  ", "-.  ", "--- ", ".--.", "--.-", ".-. ",
  "... ", "-   ", "..- ", "...-", ".-- ", "-..-", "-.--", "--.."
};

const DigitalOutputPin led(2);
const DigitalOutputPin wordLED(3);
const AnalogInputPin microphone(A0);

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

char buf[5] = {' ', ' ', ' ', ' ', '\0'};

void displayMorse(const char *str) {
  const char *c = str;
  while (*c) {
    displayChar(*c);
    delay(200);
    ++c;
  }
}

enum class ClapResult {
  NoClaps,
  OneClap,
  TwoClaps
};

bool eq(char *c1, char *c2) {
  return strcmp(c1, c2) == 0;
}

char readChar() {
  for (int i = 0; i < 4; ++i) {
    uint32_t cooldown = 0;
    ClapResult result = ClapResult::NoClaps;
    led.high();
    if (i == 0) wordLED.high();
    for (int n = 0; n < 1000; ++n) {
      int micVal = microphone.read();
      if (micVal > 600 && millis() > cooldown) {
        switch (result) {
        case ClapResult::NoClaps:
          result = ClapResult::OneClap;
          break;
        case ClapResult::OneClap:
          result = ClapResult::TwoClaps;
          break;
        case ClapResult::TwoClaps:
          result = ClapResult::TwoClaps;
          break;
        }
        cooldown = millis() + 250;
      }
      delay(1);
    }
    led.low();
    if (i == 0) wordLED.low();
    switch (result) {
    case ClapResult::NoClaps:
      buf[i] = ' ';
      break;
    case ClapResult::OneClap:
      buf[i] = '.';
      break;
    case ClapResult::TwoClaps:
      buf[i] = '-';
      break;
    }
    delay(1000);
  }
  for (uint8_t codeIdx = 0; codeIdx < 26; ++codeIdx) {
    char *code = morse[codeIdx];
    if (eq(buf, code)) return lowerAscii[codeIdx];
  }
  return '\n';
}

void setup() {
}

void loop() {
  Serial << readChar();
}