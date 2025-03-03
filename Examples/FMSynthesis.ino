/*
  FMSynthesis.ino - FM synthesis demo for a Feather board with DAC

  This sketch uses the FM synthesis module (from FMSynthesis.h) to generate an audio
  waveform and outputs it via the DAC on pin A0. The FM synthesis equation is:
      y(t) = amplitude * sin(2π * carrierPhase + modulationIndex * sin(2π * modulatorPhase))

  Notes:
  - This example assumes you're using a Feather board with a true DAC (e.g., Feather M0 Express).
  - The DAC output is 10-bit (0–1023). Adjust analogWriteResolution if needed.
  - The sample timing is approximated using delayMicroseconds(). For improved audio quality,
    consider using hardware timers or an audio library.
*/

#include "FMSynthesis.h"

// Create an instance of the FM synthesis module.
// Parameters: sample rate, carrier frequency (Hz), modulator frequency (Hz),
// modulation index, amplitude.
FMSynthesis fmSynth(44100, 440.0f, 220.0f, 2.0f, 1.0f);

// DAC output pin (using A0 on the Feather)
const int dacPin = A0;

void setup() {
  Serial.begin(115200);
  Serial.println("FM Synthesis Demo on Feather");

  // Set the DAC resolution to 10 bits (0-1023)
  analogWriteResolution(10);
}

void loop() {
  // Generate one FM synthesized sample (range: -1.0 to 1.0)
  float sample = fmSynth.generateSample();

  // Map the sample from [-1, 1] to [0, 1023] for the DAC
  int dacValue = (int)((sample + 1.0f) * 511.5f);  // 1023 / 2 = 511.5

  // Output the value to the DAC
  analogWrite(dacPin, dacValue);

  // Delay to approximate the sample period (≈22.68 µs for 44.1 kHz)
  delayMicroseconds(23);
}
