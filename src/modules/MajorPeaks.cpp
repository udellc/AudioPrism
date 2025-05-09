#include "MajorPeaks.h"

MajorPeaks::MajorPeaks()
{
    // restrict the number of peaks to find to 4
    maxNumPeaks = 4;

    // allocate memory for output array
    // output[MP_FREQ] is an array of frequencies, indexed by peak number
    // output[MP_AMP] is an array of amplitudes, indexed by peak number
    // peaks are always stored in order of increasing frequency
    // if there are fewer than maxNumPeaks peaks, the remaining elements are padded with zeros
    output          = new float*[2];
    output[MP_FREQ] = new float[maxNumPeaks];
    output[MP_AMP]  = new float[maxNumPeaks];
}

MajorPeaks::MajorPeaks(int n)
{
    // restrict the number of peaks to find to n
    maxNumPeaks = n;

    // allocate memory for output array
    // output[MP_FREQ] is an array of frequencies, indexed by peak number
    // output[MP_AMP] is an array of amplitudes, indexed by peak number
    // peaks are always stored in order of increasing frequency
    // if there are fewer than maxNumPeaks peaks, the remaining elements are padded with zeros
    output          = new float*[2];
    output[MP_FREQ] = new float[maxNumPeaks];
    output[MP_AMP]  = new float[maxNumPeaks];
}

MajorPeaks::~MajorPeaks()
{
    // free memory allocated for output arrays
    delete[] output[MP_FREQ]; // free the array of frequencies
    delete[] output[MP_AMP];  // free the array of amplitudes
    delete[] output;          // free the array managing the arrays of frequencies and amplitudes

    // free temporary storage
    delete[] outputFrequencies;
    delete[] outputAmplitudes;
}

void MajorPeaks::resetPeaksArrays()
{
    numPeaks = 0; // reset the number of peaks to zero

    if (outputLength != windowSizeBy2 >> 1) {
        delete[] outputFrequencies;
        delete[] outputAmplitudes;
        outputFrequencies = new float[windowSizeBy2 >> 1];
        outputAmplitudes  = new float[windowSizeBy2 >> 1];
    }

    // zero out the output arrays
    for (int i = 0; i < windowSizeBy2 >> 1; i++) {
        outputFrequencies[i] = 0;
        outputAmplitudes[i]  = 0;
    }
}

void MajorPeaks::findPeaks()
{
    float* windowData = spectrogram->getCurrentWindow();
    // iterate through the frequency range, excluding the first and last bins
    for (int i = lowerBinBound + 1; i < upperBinBound; i++) {
        // if the current bin is a peak, store its frequency and amplitude
        if (windowData[i] > windowData[i - 1]
            && windowData[i] > windowData[i + 1]) {
            // store the frequency of the peak
            // the index is multiplied by freqRes to convert the bin number to a frequency value
            // freqRes is the frequency width of each bin
            outputFrequencies[numPeaks] = i * freqRes;

            // store the amplitude of the peak
            outputAmplitudes[numPeaks] = windowData[i];

            // increment the number of peaks found to reflect the addition of this peak
            numPeaks++;
        }
    }
}

void MajorPeaks::trimPeaks()
{
    // loop until numPeaks <= maxNumPeaks, removing the smallest peak each iteration
    while (numPeaks > maxNumPeaks) {
        float minAmp   = outputAmplitudes[0];
        int   minIndex = 0;

        // iterate through the peaks, storing the index of the peak with the smallest amplitude
        // the peak at this index will be removed in the next step
        for (int i = 1; i < numPeaks; i++) {
            if (outputAmplitudes[i] < minAmp) {
                minAmp   = outputAmplitudes[i];
                minIndex = i;
            }
        }

        // remove the peak with the smallest amplitude
        // removal is done by shifting all elements after the removed peak one index to the left
        for (int i = minIndex; i < numPeaks - 1; i++) {
            outputFrequencies[i] = outputFrequencies[i + 1];
            outputAmplitudes[i]  = outputAmplitudes[i + 1];
        }
        numPeaks--;
    }
}

void MajorPeaks::storePeaks()
{
    // copy peaks from temporary storage to the actual output arrays
    for (int i = 0; i < maxNumPeaks; i++) {
        // if there are fewer than maxNumPeaks peaks, pad array with zeros
        if (i < numPeaks) {
            output[MP_FREQ][i] = outputFrequencies[i];
            output[MP_AMP][i]  = outputAmplitudes[i];
        } else {
            output[MP_FREQ][i] = 0;
            output[MP_AMP][i]  = 0;
        }
    }
}

void MajorPeaks::printOutput()
{
    Serial.printf("MajorPeaks [Freq, Amp]: ");
    for (int i = 0; i < maxNumPeaks - 1; i++) {
        Serial.printf("[%03g, %03g], ",
            round(output[MP_FREQ][i]),
            round(output[MP_AMP][i]));
    }
    Serial.printf("[%03g, %03g]\n",
        round(output[MP_FREQ][maxNumPeaks - 1]),
        round(output[MP_AMP][maxNumPeaks - 1]));
}

void MajorPeaks::doAnalysis()
{
    resetPeaksArrays();
    findPeaks();
    trimPeaks();
    storePeaks();

    // if debug is enabled, print the output to the serial console
    if (debugMode & DEBUG_ENABLE) {
        Serial.printf("===MAJORPEAKS===\n");
        if (debugMode & DEBUG_VERBOSE) {
            printModuleInfo();
        }
        printOutput();
        Serial.printf("================\n");
    }
}
