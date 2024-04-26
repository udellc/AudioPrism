#include "AnalysisModule.h"

// This Example assumes FFT output is accessible.
// Check README for more info on handling input, processing the FFT, and synthesizing output.
float** FFTOutput;  // dummy pointer

// declare modules
MajorPeaks bassPeaks = MajorPeaks(4);   // find 4 peaks
MajorPeaks trebelPeaks = MajorPeaks(4); // find 4 peaks

void setup() {
  // limit the frequency range for each module
  bassPeaks.setAnalysisRangeByFreq(0, 250);
  trebelPeaks.setAnalysisRangeByFreq(300, 900);

  Serial.printf("Ready\n");
}
void loop() {
    // Process input data and produce FFT output

    // Run analysis for each module
    bassPeaks.doAnalysis(FFTOutput);
    trebelPeaks.doAnalysis(FFTOutput);

    // Get MajorPeaks data
    float* bassPeakFrequencies = bassPeaks.getOutput()[MP_FREQ];
    float* bassPeakAmplitudes  = bassPeaks.getOutput()[MP_AMP];

    float* treblePeakFrequencies = treblePeaks.getOutput()[MP_FREQ];
    float* treblePeakAmplitudes = treblePeaks.getOutput()[MP_AMP];

    // Manipulate data

    // Map outputs
}
