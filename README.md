# Arduino Toys

This is just a couple of toy Arduino projects that I've enjoyed
messing around with.

## ArduinoUtils

This is a nicer C++ API around analog and digital I/O for Arduino projects.
The main benefit is the complete separation between input and output,
so there's no possible way to, say, read from an input pin accidentally.

It also uses C++ constructors to automatically set pin modes and things.

## AudioMonitor

This is a tiny project that, given 3 LEDs on pins 2, 4, and 7, and an
analog mic on pin 0, will use the 3 LEDs as a volume meter.

## Clapper

This is an implementation of the Clapper, a.k.a. a machine that
lights an LED after 2 claps.

## StayLoud

This is a dumb box that screams at you until you out-scream it.

## Morse

This is a small program that plays morse code audio of whatever string you
want.

## StopIt

This is a rigged arcade game similar to Wheel of Fortune.

# Author

Harlan Haskins ([@harlanhaskins](https://github.com/harlanhaskins))

# License

This code is all released under the MIT License, a copy of which is
available in this repository.
