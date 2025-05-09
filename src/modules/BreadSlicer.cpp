#include "BreadSlicer.h"

BreadSlicer::BreadSlicer()
{
    this->numBands = 0; // initialize number of bands to 0

    this->bandIndexes = NULL; // initialize index array to null
    this->output      = NULL; // initialize output pointer to null
}

BreadSlicer::~BreadSlicer()
{
    if (this->bandIndexes != NULL)
        free(bandIndexes);
    if (this->output != NULL)
        free(output);
}

void BreadSlicer::setBands(int* frequencyBands, int numBands)
{
    int _nyquist = sampleRate >> 1; // nyquist frequency is 1/2 the sampleRate

    // validate band boundaries are increasing and within valid range
    for (int i = 0; i < numBands; i++) {                                                                                                                      // check if each band is greater than the previous and less than next
        if (!((frequencyBands[i] >= 0 && frequencyBands[i] <= _nyquist) && (frequencyBands[i] < frequencyBands[i + 1] && frequencyBands[i + 1] <= _nyquist))) // band order and next band within valid range
        {
            Serial.println("BreadSlicer setBands() fail! Invalid bands!");
            return;
        }
    }

    // free old bandIndexes and output pointers if bands have already been set
    if (this->bandIndexes != NULL)
        free(bandIndexes); // free bandIndexes
    if (this->output != NULL)
        free(output); // free output

    // allocate memory for new bandIndexes and outpt pointers
    this->bandIndexes = (int*)malloc(sizeof(int) * numBands + 1);
    this->output      = (float*)malloc(sizeof(float) * numBands);
    this->numBands    = numBands; // set new number of bands

    // find the FFT bin index of each freq and store it in bandIndexes
    for (int i = 0; i < numBands + 1; i++) {
        bandIndexes[i] = round(frequencyBands[i] * freqWidth);
        if (i < numBands) {
            this->output[i] = 0.0; // initialize amplitude of slice to 0
        }
    }
}

void BreadSlicer::doAnalysis()
{
    if (this->bandIndexes == NULL)
        return; // do not run analysis if bands are not set

    float* windowData = spectrogram->getCurrentWindow();

    int   _bandIndex = 1;
    float _bandSum   = 0;
    // finds the total amplitude of each band by summing the bins within each band
    // then stores that value in output
    for (int i = this->bandIndexes[0]; i <= this->bandIndexes[this->numBands]; i++) {
        // if still within the band, add amp to _bandSum
        if (i < this->bandIndexes[_bandIndex]) {
            _bandSum += windowData[i];
        } else {
            // if not within band, store sum of amplitudes for the band in output array
            this->output[_bandIndex - 1] = _bandSum;
            _bandIndex += 1;
            _bandSum = 0;
            i -= 1; // Decrease i to stay within the band range during the next loop iteration
        }
    }

    // if debug is enabled, print the output to the serial console
    if (debugMode & DEBUG_ENABLE) {
        Serial.printf("===BREADSLICER===\n");
        if (debugMode & DEBUG_VERBOSE) {
            printModuleInfo();
        }
        printOutput();
        Serial.printf("=================\n");
    }
}

void BreadSlicer::printOutput()
{
    Serial.printf("BreadSlicer sums: \n");
    for (int i = 0; i < numBands; i++) {
        Serial.printf("[%d]: %f\n", i, output[i]);
    }
}
