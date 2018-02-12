#include "ArduinoUtils.h"

ShiftRegister leds(2, 3);
DigitalInputPin button(4);
DigitalOutputPin buzzer(5);
const int timeout = 30;
const uint8_t winningLED = 4;

bool isRigged = false;

void setup() {
  randomSeed(analogRead(0));
}

/// Moves the currently selected LED from the provided LED to the provided other
/// LED. Each LED in a row is displayed, so it looks inconspicuous and a product
/// of poor timing.
void animateLEDMove(uint8_t from, uint8_t to) {
  while (from != to) {
    leds.low(from);
    from += from > to ? -1 : 1;
    leds.high(from);
    leds.write();
  }
}

/// Sound the buzzer, signifying a loss.
void lose() {
  Serial << "You LOSE!\n";
  buzzer.high();
  delay(250);
  buzzer.low();
  delay(750);
}

/// Display a winning message via serial.
void win() {
  Serial << "You WIN!\n";
  delay(1000);
}

/// Always win! If you weren't on the winning LED, move the LED over to the
/// winning one and then display the win message.
bool riggedHandlePress(bool down, uint8_t led) {
  animateLEDMove(led, winningLED);
  win();
  return true;
}

/// Handles a press of a given LED. If the LED stops on the winning light,
/// then 70% of the time move it up or down to a losing LED.
void handlePress(bool down, uint8_t led) {
  if (led == winningLED) {
    if (random(10) < 7) {
      Serial << "Sorry bud, not your lucky day. Making you lose.\n";
      animateLEDMove(led, led + (down ? -1 : 1));
      lose();
    } else {
      win();
    }
  } else {
    lose();
  }
}

/// Waits for 250ms after pressing the button to see if the user
/// releases and then re-presses the button. This will enable or disable
/// rigging the game.
bool detectDoublePress() {
  bool wasLow = false;
  for (int n = 0; n < 250; ++n) {
    delay(1);
    bool isHigh = button.isHigh();
    if (!wasLow && !isHigh) {
      wasLow = true;
      continue;
    }
    if (wasLow && isHigh) {
      isRigged = !isRigged;
      Serial << "game is now " << (isRigged ? "" : "not ") << "rigged\n";
      delay(100);
      return true;
    }
  }
  return false;
}

/// The loop() function starts a new round of the stopit game.
void loop() {
  leds.clear();
  bool down = false;
  int currentLED = 0;

  /// Forever, display LEDs one at a time from left-to-right and then from
  /// right-to-left. Each time a new LED is lit, in the timeout period while
  /// the LED is lit, attempt to detect a button press --- then, if the game
  /// is rigged, move the light back to the winning position and stop it,
  /// signifying a win.
  /// Otherwise, the game is rigged in the opposite direction.
  while (true) {
    leds.high(currentLED);
    leds.write();
    for (int n = 0; n < timeout; ++n) {
      delay(1);
      if (button.isLow()) continue;
      if (isRigged) {
        bool wasRigged = riggedHandlePress(down, currentLED);
        detectDoublePress();
        handlePress(down, wasRigged ? winningLED : currentLED);
      } else if (!detectDoublePress()) {
        handlePress(down, currentLED);
      }
      return;
    }
    leds.low(currentLED);

    currentLED += (down ? -1 : 1);
    if (currentLED == 7) { down = true; }
    if (currentLED == 0) { down = false; }
  }
}