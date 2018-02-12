#include "ArduinoUtils.h"

DigitalInputPin buttons[3] = {
  {2}, {3}, {4}
};
DigitalOutputPin leds[3] = {
  {5}, {6}, {7}
};

#define NUM_MOVES 24

void setup() {
  randomSeed(analogRead(0));
  reset();
}

uint8_t randomMove() {
  return random(0, 3);
}

int moves[NUM_MOVES];
int currentMove = 0;

#define forEachMove(stmts) \
  for (int i = 0; i < currentMove; ++i) { \
    auto move = moves[i]; \
    stmts \
  }

void reset() {
  currentMove = 0;
}

void lightPin(int idx) {
  leds[idx].high();
  delay(500);
  leds[idx].low();
  delay(500);
}

void showMoves() {
  forEachMove({
    lightPin(move);
  });
}

void dump() {
  Serial << "current move is " << currentMove << "\n";
  Serial << "moves: [";
  bool isFirst = true;
  forEachMove({
    if (!isFirst) { Serial << ", "; }
    isFirst = false;
    Serial << move;
  })
  Serial << "]\n";
}

void readMoves() {
  int current = 0;
  while (current < currentMove) {
    for (int i = 0; i < 3; ++i) {
      DigitalInputPin button = buttons[i];
      if (!button.isHigh()) continue;

      DigitalOutputPin led = leds[i];
      led.high();
      while (button.isHigh()) { delay(1); }
      led.low();

      if (moves[current] != button.pinNum) {
        showFailure();
        reset();
        return;
      }

      ++current;
      break;
    }
  }
}

void showFailure() {
  Serial << "FAIL\n";
  for (int i = 0; i < 3; ++i) {
    leds[0].high();
    leds[1].high();
    leds[2].high();
    delay(200);
    leds[0].low();
    leds[1].low();
    leds[2].low();
    delay(200);
  }
}

void loop() {
  moves[currentMove] = randomMove();
  currentMove += 1;
  dump();
  showMoves();
  readMoves();
  delay(1000);
}