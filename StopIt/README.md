# StopIt - A Rigged Arcade Game

StopIt is an implementation of a Wheel of Fortune-style rigged arcade game.
The object is to press the button as soon as the red LED is lit, but in "normal
mode," the game will move you to the adjacent LED 70% of the time if you win.

When you double-press the button, the game becomes rigged the other way. No
matter which LED you stop on, the system will animate them back over to the
winning LED and tell you that you've won.

## Implementation

This project requires:

- One SN74LS164 8-bit parallel out shift register
- Eight LEDs, one red and 7 green
- Two 500ohm resistor nets
- One 560ohm resistor
- One push-button
- One buzzer (optional)

Wire up the inputs such that the eight LEDs are receiving power, in order, from
pins A-H on the shift register.

Connect the two reistor nets on the same pins as the LED, and ground them from
the common ground.

Ground the shift register, and connect either data pin of the shift register to
pin 2 on the arduino. Connect the clock pin to pin 3 on the Arduino.

Connect the button to power from one side and connect it to ground via the
560ohm resistor. On the same line as the ground pin, connect that to pin 4 on
the rduino.

Optionally, connect the buzzer to any free line, and power it from pin 5 on the
Arduino.

Compile and run the code, available
[on GitHub here](https://github.com/harlanhaskins/Arduino-Toys/tree/master/StopIt).

The board should look something like this:

<img src='https://pcomp.harlanhaskins.com/assets/images/stopit-board.jpg'/>
