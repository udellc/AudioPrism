#include "TotalAmplitude.h"

void TotalAmplitude::doAnalysis()
{
    float* windowData = spectrogram->getCurrentWindow();

    // initialize total to 0, the minimum possible amplitude sum
    float total = 0.0;

    // iterate through the bins in the selected frequency range, adding the amplitude of each bin to the total
    for (int i = lowerBinBound; i < upperBinBound; i++) {
        // total += curWindow[i];
        total += windowData[i];
    }

    // store the total amplitude in the output variable
    // the output of this module can be retrieved by calling getOutput() after analysis
    output = total;

    // if debug is enabled, print the output to the serial console
    if (debugMode & DEBUG_ENABLE) {
        Serial.printf("===TOTAL_AMPLITUDE===\n");
        if (debugMode & DEBUG_VERBOSE) {
            printModuleInfo();
        }
        Serial.printf("Total: %f\n", total);
        Serial.printf("=====================\n");
    }
}
