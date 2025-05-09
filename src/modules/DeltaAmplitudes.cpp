#include "DeltaAmplitudes.h"

DeltaAmplitudes::DeltaAmplitudes()
{
    deltaAmplitudes = new float[windowSize];
}

DeltaAmplitudes::~DeltaAmplitudes()
{
    delete[] deltaAmplitudes;
}

void DeltaAmplitudes::doAnalysis()
{
    float* currWindowData = spectrogram->getCurrentWindow();
    float* prevWindowData = spectrogram->getPreviousWindow();

    // iterate through FFT data and store the change in amplitudes between current and previous window
    for (int i = lowerBinBound; i < upperBinBound; i++) {
        deltaAmplitudes[i] = abs(currWindowData[i] - prevWindowData[i]);
    }

    output = deltaAmplitudes;

    // if debug is enabled, print the output to the serial console
    if (debugMode & DEBUG_ENABLE) {
        Serial.printf("===DELTA_AMPLITUDES===\n");
        if (debugMode & DEBUG_VERBOSE) {
            printModuleInfo();
        }
        printOutput();
        Serial.printf("======================\n");
    }
}

void DeltaAmplitudes::printOutput()
{
    Serial.printf("Delta Amplitudes: ");
    for (int i = lowerBinBound; i < upperBinBound; i++) {
        Serial.printf("%01g, ", round(output[i]));
        i += 3;
    }
    Serial.printf("\n");
}
