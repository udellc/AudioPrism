#include "SalientFreqs.h"

SalientFreqs::SalientFreqs()
{
    numFreqs     = 3; // default to finding frequency of max change
    salientFreqs = new int[numFreqs];
    this->addSubmodule(&deltaAmps);

    for (int i = 0; i < numFreqs; i++) {
        salientFreqs[i] = -1;
    }
}

SalientFreqs::SalientFreqs(int n)
{
    if (n < 0) {
        n = 0;
    }
    if (n > windowSizeBy2) {
        n = windowSizeBy2;
    }

    numFreqs     = n;
    salientFreqs = new int[numFreqs];
    this->addSubmodule(&deltaAmps);

    for (int i = 0; i < numFreqs; i++) {
        salientFreqs[i] = -1;
    }
}

SalientFreqs::~SalientFreqs()
{
    delete[] salientFreqs;
}

void SalientFreqs::changeNumFreqs(int newSize)
{
    numFreqs = newSize;
    delete[] salientFreqs;
    salientFreqs = new int[numFreqs];
}

void SalientFreqs::doAnalysis()
{
    deltaAmps.doAnalysis();
    amplitudes = deltaAmps.getOutput(); // copy amplitudes

    // iterate through amplitudes to find the maximum(s)
    int currMaxAmp    = 0;
    int currMaxAmpIdx = -1;
    for (int i = 0; i < numFreqs; i++) {
        for (int j = lowerBinBound; j < upperBinBound; j++) {
            if (amplitudes[j] > currMaxAmp) {
                currMaxAmp    = amplitudes[j];
                currMaxAmpIdx = j;
            }
        }
        salientFreqs[i]           = currMaxAmpIdx; // add the new max amp index to the array
        amplitudes[currMaxAmpIdx] = 0;             // set the amp to 0 so it will not be found again
        currMaxAmp                = 0;             // reset iterators
        currMaxAmpIdx             = -1;
    }
    output = salientFreqs;

    // if debug is enabled, print the output to the serial console
    if (debugMode & DEBUG_ENABLE) {
        Serial.printf("===SALIENT_FREQS===\n");
        if (debugMode & DEBUG_VERBOSE) {
            printModuleInfo();
        }
        Serial.printf("Salient Freqs: ");
        for (int i = 0; i < numFreqs; i++) {
            Serial.printf("%d ", salientFreqs[i]);
        }
        Serial.printf("\n==========================\n");
    }
}

void SalientFreqs::printOutput()
{
}
