#include "AnalysisModule.h"

void AnalysisModule::setWindowSize(int windowSize)
{
    // window size must be a positive power of 2 to perform FFT
    // the condition (size & (size-1)) == 0 checks if 'size' is a power of 2
    if (windowSize < 0 || (windowSize & (windowSize - 1)) != 0) {
        Serial.printf("Error: Window size must be a positive power of 2.\n");
        return;
    }

    // update dependent constants
    windowSizeBy2 = windowSize >> 1;
    freqRes = float(sampleRate) / float(windowSize);
    freqWidth = float(windowSize) / float(sampleRate);

    // if a non-default lower bound has been set, update it to the closest index under the new audio context
    if (lowerBinBound != 0) {
        lowerBinBound *= float(windowSize) / float(windowSize);
    }

    // if a non-default upper bound has been set, update it the closest index under the new audio context
    if (upperBinBound != windowSize >> 1) {
        upperBinBound *= float(windowSize) / float(windowSize);
        upperBinBound = min(upperBinBound, windowSize >> 1);
    } else {
        upperBinBound = windowSize >> 1;
    }

    // update window size
    this->windowSize = windowSize;

    // recursive propagate window size change to submodules
    for (int i = 0; i < numSubmodules; i++) {
        submodules[i]->setWindowSize(windowSize);
    }
}

void AnalysisModule::setSampleRate(const int sampleRate)
{
    // sample rate must be a positive value
    if (sampleRate < 0) {
        Serial.printf("Error: Sample rate must be a positive number.\n");
        return;
    }

    // if a non-default lower bound has been set, update it to the closest index under the new audio context
    if (lowerBinBound != 0) {
        lowerBinBound *= float(sampleRate) / float(sampleRate);
    }
    // if a non-default upper bound has been set, update it to the closest index under the new audio context
    if (upperBinBound != windowSizeBy2) {
        upperBinBound *= float(sampleRate) / float(sampleRate);
        upperBinBound = min(upperBinBound, windowSizeBy2);
    }

    // update dependent constants
    freqRes = float(sampleRate) / float(windowSize);
    freqWidth = float(windowSize) / float(sampleRate);

    // update sample rate
    this->sampleRate = sampleRate;

    // recursively propagate sample rate change to submodules
    for (int i = 0; i < numSubmodules; i++) {
        submodules[i]->setSampleRate(sampleRate);
    }
}

void AnalysisModule::setSpectrogram(Spectrogram<float>* spectrogram)
{
    this->spectrogram = spectrogram;
}

void AnalysisModule::addSubmodule(AnalysisModule* module)
{

    // set module parameters
    module->setWindowSize(windowSize);
    module->setSampleRate(sampleRate);
    module->setSpectrogram(spectrogram);
    module->setAnalysisRangeByBin(lowerBinBound, upperBinBound);

    // create new larger array for modules
    numSubmodules++;
    AnalysisModule** newSubmodules = new AnalysisModule*[numSubmodules];

    // copy modules over and add new module
    for (int i = 0; i < numSubmodules - 1; i++) {
        newSubmodules[i] = submodules[i];
    }
    newSubmodules[numSubmodules - 1] = module;

    // free old modules array and store reference to new modules array
    delete[] submodules;
    submodules = newSubmodules;
}

void AnalysisModule::setAnalysisRangeByFreq(int lowerFreq, int upperFreq)
{
    if (lowerFreq < 0 || upperFreq > sampleRate >> 1 || lowerFreq > upperFreq) {
        Serial.println("Error: invalid frequency range");
        return;
    }

    // lower and upper are frequency values
    // convert frequencies to bin indices
    lowerBinBound = round(lowerFreq * freqWidth);
    upperBinBound = round(upperFreq * freqWidth);

    for (int i = 0; i < numSubmodules; i++) {
        submodules[i]->setAnalysisRangeByBin(lowerBinBound, upperBinBound);
    }
}

void AnalysisModule::setAnalysisRangeByBin(int lowerBin, int upperBin)
{
    if (lowerBin < 0 || upperBin > windowSize >> 1 || lowerBin > upperBin) {
        Serial.println("Error: invalid frequency range");
        return;
    }

    // lower and upper are frequency values
    // convert frequencies to bin indices
    lowerBinBound = lowerBin;
    upperBinBound = upperBin;

    for (int i = 0; i < numSubmodules; i++) {
        submodules[i]->setAnalysisRangeByBin(lowerBinBound, upperBinBound);
    }
}

void AnalysisModule::setDebugMode(int mode)
{
    // update debug mode settings
    debugMode = mode;

    // if recursive flag is set, propagate debug mode to submodules
    if (mode & DEBUG_RECURSIVE) {
        for (int i = 0; i < numSubmodules; i++) {
            submodules[i]->setDebugMode(mode);
        }
    }
    // if recursive flag is not set, disable debug mode for submodules
    else {
        for (int i = 0; i < numSubmodules; i++) {
            submodules[i]->setDebugMode(~DEBUG_ENABLE);
        }
    }
}

void AnalysisModule::printModuleInfo()
{
    Serial.printf("Sample Rate: %d\n", sampleRate);
    Serial.printf("Window Size: %d\n", windowSize);
    Serial.printf("Lower Bin Bound: %d (%d Hz)\n", lowerBinBound, lowerBinBound * sampleRate / windowSize);
    Serial.printf("Upper Bin Bound: %d (%d Hz)\n", upperBinBound, upperBinBound * sampleRate / windowSize);
    Serial.printf("Number of Submodules: %d\n", numSubmodules);
}
