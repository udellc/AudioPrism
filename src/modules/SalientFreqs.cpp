#include "SalientFreqs.h"

SalientFreqs::SalientFreqs()
{
    numFreqs     = 3; // default to finding frequency of max change
    direction    = 0;
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
    direction    = 0;
    salientFreqs = new int[numFreqs];
    this->addSubmodule(&deltaAmps);

    for (int i = 0; i < numFreqs; i++) {
        salientFreqs[i] = -1;
    }
}
SalientFreqs::SalientFreqs(int n, int dir)
{
    // If n is less than zero set it to zero so that it doesn't break
    if (n < 0) {
        n = 0;
    }
    // If n is greater than the bin range given by windowSizeBy2 truncate it
    if (n > windowSizeBy2) {
        n = windowSizeBy2;
    }
    // If dir isn't one of the directional choice (0, 1, 2)
    if (dir < 0 || dir > 2) {
        dir = 0;
    }

    direction = dir;

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

void SalientFreqs::changeDirction(int dir)
{
    direction = dir;
}

bool SalientFreqs::checkDirection(int idx)
{
    float* current_window = spectrogram->getCurrentWindow();
    float* prev_window    = spectrogram->getPreviousWindow();
    if (direction == 1 && current_window[idx] > prev_window[idx])
        return false;
    if (direction == 2 && current_window[idx] < prev_window[idx])
        return false;
    return true;
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
                if (checkDirection(j)) {
                    currMaxAmp    = amplitudes[j];
                    currMaxAmpIdx = j;
                }
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
