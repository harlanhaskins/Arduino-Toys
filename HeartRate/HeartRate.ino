#include "ArduinoUtils.h"
#include <Adafruit_NeoPixel.h>

#define BEAT_THRESHOLD 550
#define NUM_PIXELS 14

/// Represents a NeoPixel and its current brightness value.
/// It will be on for a set duration, going from 100% to 0% linearly over
/// the duration.
struct Light {
  /// The red light value, starting at 0 brightness.
  uint8_t redValue = 0;

  /// The time that this light should start at 100%.
  uint32_t startMillis = 0;
  
  /// The duration the light should be on.
  uint32_t duration = 0;

  Light() {}

  /// Tells the light to start at `startMillis` and last for `duration`.
  void setDisplayTime(uint32_t startMillis, uint32_t duration) {
    this->startMillis = startMillis;
    this->duration = duration;
  }

  /// Each tick, update the red value of the light if the current time is
  /// between startMillis and (startMillis + duration). 
  void update() {
    if (startMillis == 0) return;
    auto ms = millis();
    if (ms < startMillis) return;
    uint32_t diff = ms - startMillis;
    if (diff > duration) {
      startMillis = duration = 0;
      redValue = 0;
      return;
    }
    // Set the red value to a linear value between 0 and 255 depending what
    // percentage through the duration we've gone. 
    redValue = map(diff, duration, 0, 0, 255);
  }
};

/// A BeatDisplayer schedules light pulses when a beat happens.
template <int NumLights>
class BeatDisplayer {
  /// An array of light objects representing the current state of all the
  /// lights.
  Light lights[NumLights];

  /// The neopixel strip to control when a beat happens.
  Adafruit_NeoPixel &strip;

public:

  /// Creates a BeatDisplayer that will display beats on the given strip.
  BeatDisplayer(Adafruit_NeoPixel &strip): strip(strip) {}

  /// Updates the neopixel strip with the current set of light values.
  void writePixels() {
    for (int i = 0; i < NumLights; ++i) {
      strip.setPixelColor(i, lights[i].redValue, 0, 0);
    }
    strip.show();
  }

  /// When there is a beat, schedule a pulse down the lights.
  /// Each light will last for half the estimated time of the heartbeat,
  /// and will be delayed based on the number of lights in the strip.
  void beat(int duration) {
    int lightDuration = duration / 2;
    int delayMs = lightDuration / NumLights;
    for (uint32_t i = 0; i < NumLights; ++i) {
      lights[i].setDisplayTime(millis() + (i * delayMs), lightDuration);
    }
  }

  /// Every tick, update each of the lights and write the value to the strip.
  void update() {
    for (int i = 0; i < NumLights; ++i) {
      lights[i].update();
    }
    writePixels();
  }
};

/// BeatTracker tracks peaks in the heart rate and determines a rough estimate
/// of the user's BPM.
class BeatTracker {
  /// The time of the previous peak above the threshold.
  uint32_t prevPeakMs = 0;

  /// If we're currently cooling down, at what point we should stop cooling
  /// down.
  uint32_t cooldownMillis = 0;

  /// The sensor to read from.
  AnalogInputPin &sensor;
public:
  /// Creates a BeatTracker listening to the given heart rate sensor.
  BeatTracker(AnalogInputPin &sensor): sensor(sensor) {}

  /// Updates the beat tracker, and returns a non-zero integer with the duration
  /// of the previous heart beat if it detected a beat.
  int update() {
    auto ms = millis();
    if (ms < cooldownMillis) return 0;
    cooldownMillis = 0;
    auto value = sensor.read();
    if (value > BEAT_THRESHOLD) {
      cooldownMillis = ms + 200;
      int duration = ms - prevPeakMs;
      prevPeakMs = ms;
      return duration;
    }
    return 0;
  }
};

AnalogInputPin sensor(A0);
Adafruit_NeoPixel strip(NUM_PIXELS, 0, NEO_GRB + NEO_KHZ800);
BeatDisplayer<NUM_PIXELS> beatDisplayer(strip);
BeatTracker tracker(sensor);

void setup() {
  strip.begin();
  strip.show();
}

void loop() {
  // If the tracker gave us a beat duration, tell the beat displayer to show
  // a beat with that duration.
  if (auto duration = tracker.update()) {
    beatDisplayer.beat(duration);
  }

  // Update the beat displayer to ensure the pixels are drawn.
  beatDisplayer.update();
  delay(1);
}