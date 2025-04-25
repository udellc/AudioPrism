#include "MeanAmplitude.h"

MeanAmplitude::MeanAmplitude()
{
    this->addSubmodule(&totalAmp);
}

// doAnalysis() is called by the analysis manager
// the totalamplitude submodule is invoked to calculate the total amplitude of the current window
// the mean amplitude is calculated from the total amplitude and the number of bins in the selected frequency range
void MeanAmplitude::doAnalysis()
{
    // perform analysis on the totalamplitude module
    totalAmp.doAnalysis();

    // retrieve the output of the totalamplitude module
    float total = totalAmp.getOutput();

    // calculate the mean amplitude by dividing the total amplitude by the number of bins in the selected frequency range
    // this module's output can be retrieved by calling getOutput() after analysis
    output = total / (upperBinBound - lowerBinBound);

    // if debug is enabled, print the output to the serial console
    if (debugMode & DEBUG_ENABLE) {
        Serial.printf("===MEAN_AMPLITUDE===\n");
        if (debugMode & DEBUG_VERBOSE) {
            printModuleInfo();
        }
        Serial.printf("Mean: %f\n", output);
        Serial.printf("====================\n");
    }
}
