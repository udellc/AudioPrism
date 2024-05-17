#include "AnalysisModule.h"

// This Example assumes FFT output is accessible.
// Check README for more info on handling input, processing the FFT, and synthesizing output.
float** FFTOutput;  // dummy pointer

// declare modules
BreadSlicer slicer = BreadSlicer();

void setup() {
  // Example frequency bands
  int bands[] = {0, 200, 500, 2000, 4000};

  // Set the number of bands and their frequencies
  slicer.setBands(bands, 4);

  Serial.printf("Ready\n");
}
void loop() {
    // Process input data and produce FFT output

    // Run analysis for each module
    slicer.doAnalysis(FFTOutput);

    // Get BreadSlicer amplitudes
    float* amps = slicer.getOutput();

    float ampSum0to200 = amps[0];   // Get a single sum

    // Manipulate data

    // Map outputs
}
