#include "Noisiness.h"

Noisiness::Noisiness()
{
    this->addSubmodule(&totalAmp); // register the TotalAmplitude submodule
}

void Noisiness::doAnalysis()
{
    totalAmp.doAnalysis();              // perform analysis with the TotalAmplitude submodule
    float total = totalAmp.getOutput(); // retrieve the output of the TotalAmplitude submodule

    float* windowData = spectrogram->getCurrentWindow();

    float entropy = 0;
    // for each bin, calculate the contribution to the overall entropy
    for (int i = lowerBinBound; i < upperBinBound; i++) {
        // if the amplitude of the current bin is zero, skip it (log2(0) is undefined)
        if (windowData[i] > 0) {
            // assign a probability to each bin based on its amplitude
            float p = windowData[i] / total;
            // calculate the contribution of this bin to the entropy
            entropy -= p * log2(p);
        }
    }

    // Normalize the entropy to a 0.-1. scale
    output = entropy / log2(upperBinBound - lowerBinBound);

    // if debug is enabled, print the output to the serial console
    if (debugMode & DEBUG_ENABLE) {
        Serial.printf("===NOISINESS===\n");
        if (debugMode & DEBUG_VERBOSE) {
            printModuleInfo();
        }
        Serial.printf("Noise: %f\n", output);
        Serial.printf("===============\n");
    }
}
