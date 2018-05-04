#include "ArduinoUtils.h"
#include <avr/io.h>
#include <Tone.h>

Tone speaker;

#define BASE_C 16.35
#define BASE_D 18.35
#define BASE_E 20.60
#define BASE_F 21.83
#define BASE_G 24.50
#define BASE_A 27.50
#define BASE_B 30.87

float baseNotes[7] = {
  BASE_C, BASE_D, BASE_E, BASE_F, BASE_G, BASE_A, BASE_B
};

int frequency(float note, int octave) {
  return int(powf(2, octave) * note);
}

void setup() {
  Serial.begin(9600);
  speaker.begin(11);
}

void loop() {
  while (Serial.available() > 0) {
    int leftRaw = Serial.parseInt();
    int rightRaw = Serial.parseInt();
    int noteIdx = map(leftRaw, 0, 255, 0, 7);
    Serial << "found note index: " << noteIdx << "\n";
    int octave = map(rightRaw, 0, 255, 0, 8);
    Serial << "found octave: " << octave << "\n";
    int freq = frequency(baseNotes[noteIdx], octave);
    Serial << "pow(2, " << octave << ") * " << baseNotes[noteIdx] << " = " << freq << "\n";
    Serial.println(freq);
    speaker.play(freq);
  }
}
