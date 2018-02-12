#include <Arduino.h>

#ifndef ARDUINO_UTILS_H
#define ARDUINO_UTILS_H

/// DigitalPin is a base class for digital I/O pins. On its own, it just
/// declares the data layout -- it's up to subclasses to implement features
/// that are specific to input or output.
class DigitalPin {
public:
  const uint8_t pinNum;
protected:
  DigitalPin(uint8_t pin): pinNum(pin) {}
};

// Forward-declaration, for DigitalInputPin::toOutput();
class DigitalOutputPin;

/// Represents a numbered digial pin on the Arduino. It allows for reading the
/// current values of a pin, and exposes no functionality for writing beyond
/// enabling the internal pullup register.
///
/// You can convert an Input pin to an Output pin by calling
/// `DigitalInputPin::toOutput()`
class DigitalInputPin : public DigitalPin {
public:
  /// Creates a reference to a digital pin and sets it up for input.
  DigitalInputPin(uint8_t pin): DigitalPin(pin) {
    setup();
  }

  /// Explicitly set up an input pin for reading input.
  void setup() const {
    pinMode(pinNum, INPUT);
  }

  /// Converts the current pin to an output pin and sets the mode to Output.
  DigitalOutputPin toOutput() const;

  /// Enables the internal pullup register and sets the pin's mode to
  /// `INPUT_PULLUP`
  void enablePullup() const {
    pinMode(pinNum, INPUT_PULLUP);
    digitalWrite(pinNum, HIGH);
  }

  /// Disables the internal pullup register and sets the pin's mode back to
  /// `INPUT`
  void disablePullup() const {
    digitalWrite(pinNum, LOW);
    pinMode(pinNum, INPUT);
  }

  /// Reads the voltage on the pin and returns `true` if it's high.
  bool isHigh() const {
    return digitalRead(pinNum) == HIGH;
  }

  /// Reads the voltage on the pin and returns `true` if it's low.
  bool isLow() const {
    return digitalRead(pinNum) == LOW;
  }

  /// Reads a pulse from the pin, starting as soon as the pin is high and
  /// timing the duration that the pin remains high. As soon as the pin
  /// is low, the timer stops and the length of the pulse, in microseconds,
  /// is returned.
  int readHighPulse(unsigned long timeout = 1000000UL) const {
    return pulseIn(pinNum, HIGH, timeout);
  }

  /// Reads a pulse from the pin, starting as soon as the pin is low and
  /// timing the duration that the pin remains low. As soon as the pin
  /// is high, the timer stops and the length of the pulse, in microseconds,
  /// is returned.
  int readLowPulse(unsigned long timeout = 1000000UL) const {
    return pulseIn(pinNum, LOW, timeout);
  }
};

/// Represents a numbered digital pin along the Arduino. It allows for writing
/// high or low voltages to a pin, and exposes no functionality for reading the
/// pin's voltage.
///
/// You can convert an Output pin to an Input pin by calling
/// `DigitalOutputPin::toInput()`
class DigitalOutputPin : public DigitalPin {
public:
  /// Creates a reference to a digital pin and sets it up for output.
  DigitalOutputPin(uint8_t pin): DigitalPin(pin) {
    setup();
  }

  /// Explicitly set up an input pin for reading input.
  void setup() const {
    pinMode(pinNum, OUTPUT);
  }

  /// Creates a reference to an input pin and sets up this pin for
  /// input.
  /// Note: It is a programmer error to use this pin for output again, after
  ///       converting it to an input pin.
  DigitalInputPin toInput() const {
    return DigitalInputPin(pinNum);
  }

  /// Sets the pin to `HIGH` if the provided boolean is `true`, otherwise
  /// sets it to `LOW`.
  void highIf(bool isOn) const {
    digitalWrite(pinNum, isOn ? HIGH : LOW);
  }

  /// Sets the pin to `HIGH`.
  void high() const {
    digitalWrite(pinNum, HIGH);
  }

  /// Sets the pin to `LOW`.
  void low() const {
    digitalWrite(pinNum, LOW);
  }

  void tone(unsigned int frequency, unsigned long duration = 0) const {
    ::tone(pinNum, frequency, duration);
  }

  void stopTone() {
    noTone(pinNum);
  }

  /// Writes an analog value (PWM wave) to a pin. Can be used to light a LED at
  /// varying brightnesses or drive a motor at various speeds. After a call to
  /// `writeAnalog()`, the pin will generate a steady square wave of the
  /// specified duty cycle until the next call to `writeAnalog()` (or a read)
  /// on the same pin. The frequency of the PWM signal on most pins is
  /// approximately 490 Hz. On the Uno and similar boards, pins 5 and 6 have a
  /// frequency of approximately 980 Hz.
  ///
  /// On most Arduino boards (those with the ATmega168 or ATmega328P), this
  /// function works on pins 3, 5, 6, 9, 10, and 11. On the Arduino Mega, it
  /// works on pins 2 - 13 and 44 - 46. Older Arduino boards with an ATmega8
  /// only support analogWrite() on pins 9, 10, and 11.
  /// The Arduino DUE supports analogWrite() on pins 2 through 13, plus pins
  /// DAC0 and DAC1. Unlike the PWM pins, DAC0 and DAC1 are Digital to Analog
  /// converters, and act as true analog outputs.
  /// You do not need to call pinMode() to set the pin as an output before
  /// calling `writeAnalog()`.
  void writeAnalog(int value) const {
    analogWrite(pinNum, value);
  }
};

DigitalOutputPin DigitalInputPin::toOutput() const {
  return DigitalOutputPin(pinNum);
}

/// Represents a numbered analog pin along the Arduino. It allows for reading
/// current values of a pin, and exposes no functionality for writing beyond
/// enabling the internal pullup register.
class AnalogInputPin {
public:
  const uint8_t pinNum;
  AnalogInputPin(uint8_t pin): pinNum(pin) {}
  
  /// Configures the reference voltage used for analog input (i.e. the value
  /// used as the top of the input range). The options are:
  /// - DEFAULT: the default analog reference of 5 volts (on 5V Arduino boards)
  ///            or 3.3 volts (on 3.3V Arduino boards)
  /// - INTERNAL: a built-in reference, equal to 1.1 volts on the ATmega168 or
  ///             ATmega328 and 2.56 volts on the ATmega8 (not available on the
  ///             Arduino Mega)
  /// - INTERNAL1V1: a built-in 1.1V reference (Arduino Mega only)
  /// - INTERNAL2V56: a built-in 2.56V reference (Arduino Mega only)
  /// - EXTERNAL: the voltage applied to the AREF pin (0 to 5V only) is used as
  ///             the reference.
  static void setReferenceVoltageMode(uint8_t mode) {
    analogReference(mode);
  }

  /// Reads an analog value (0 to 1023) from the analog pin.
  int read() const {
    return analogRead(pinNum);
  }
};

template <typename T>
Stream &operator<<(Stream &stream, T x) {
  stream.print(x);
  return stream;
}

#endif // ARDUINO_UTILS_H
