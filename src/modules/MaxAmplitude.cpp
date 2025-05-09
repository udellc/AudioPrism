#include "MaxAmplitude.h"

void MaxAmplitude::doAnalysis()
{
    float* windowData = spectrogram->getCurrentWindow();

    // initialize max to 0 (there cannot be negative amplitudes)
    float max = 0.0;

    // iterate through the bins in the selected frequency range
    // if a bin's amplitude is greater than max, update max
    // at the end of the loop, max will contain the amplitude of the highest bin
    for (int i = lowerBinBound; i < upperBinBound; i++) {
        if (windowData[i] > max) {
            max = windowData[i];
        }
    }

    // store the max amplitude in the output variable
    // the output of this module can be retrieved by calling getOutput() after analysis
    output = max;

    // if debug is enabled, print the output to the serial console
    if (debugMode & DEBUG_ENABLE) {
        Serial.printf("===MAX_AMPLITUDE===\n");
        if (debugMode & DEBUG_VERBOSE) {
            printModuleInfo();
        }
        Serial.printf("Max: %f\n", max);
        Serial.printf("===================\n");
    }
}
