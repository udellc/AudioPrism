/*
  FMSynthesis.ino - FM synthesis demo for the ESP32

  This sketch uses the FM synthesis module (from FMSynthesis.h) to generate an audio
  waveform and outputs it via the DAC on the ESP32. The FM synthesis equation used is:
      y(t) = amplitude * sin(2π * carrierPhase + modulationIndex * sin(2π * modulatorPhase))

  The generated waveform (range: -1 to 1) is mapped to an 8-bit value (0–255) suitable for the
  ESP32 DAC. This sketch uses GPIO25 (DAC channel 1) for audio output and approximates a 44.1 kHz sample rate.
*/

#include "FMSynthesis.h"

// Create an instance of the FM synthesis module.
// Parameters: sample rate, carrier frequency (Hz), modulator frequency (Hz),
// modulation index, amplitude.
FMSynthesis fmSynth(44100, 440.0f, 220.0f, 2.0f, 1.0f);

// Define the DAC output pin (GPIO25 is DAC channel 1 on the ESP32)
const int dacPin = 25;

void setup() {
  Serial.begin(115200);
  Serial.println("FM Synthesis Demo on ESP32");
}

void loop() {
  // Generate one FM synthesized sample (output range: -1.0 to 1.0)
  float sample = fmSynth.generateSample();

  // Map the sample from [-1, 1] to [0, 255] for the DAC output
  int dacValue = (int)((sample + 1.0f) * 127.5f);  // 255 / 2 = 127.5

  // Output the value to the DAC using dacWrite()
  dacWrite(dacPin, dacValue);

  // Delay to approximate the sample period (~23 microseconds for 44.1 kHz)
  delayMicroseconds(23);
}

