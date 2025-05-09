#include "Centroid.h"

void Centroid::doAnalysis()
{
    float* windowData = spectrogram->getCurrentWindow();
    // get the sum of amplitudes and sum of frequencies*amplitudes
    float ampSum     = 0; // total amp across bins
    float freqAmpSum = 0;
    int   amp, freq;
    // calculate center frequency of the bin
    // i * freqRes gives lower edge, adding freqResBy2 gives center
    for (int i = lowerBinBound; i < upperBinBound; i++) {
        amp  = windowData[i];
        freq = (i * freqRes + freqResBy2); // use the center frequency of a bin
        ampSum += amp;                     // get total amp
        freqAmpSum += freq * amp;          // get total freq * amp (weighted sum)
    }
    // if ampSum 0, set centroid to 0 -- this means no signal
    // otherwise divide weighted frequency sum by total amplitude -- "center of mass"
    // lower centroid tends to be darker bassier sounds, higher centroids tend to be brighter sounds
    centroid = (ampSum == 0) ? 0 : (freqAmpSum / ampSum);
    output   = centroid;

    // if debug is enabled, print the output to the serial console
    if (debugMode & DEBUG_ENABLE) {
        Serial.printf("===CENTROID===\n");
        if (debugMode & DEBUG_VERBOSE) {
            printModuleInfo();
        }
        Serial.printf("Amplitude Sum: %f\n", ampSum);
        Serial.printf("Freq. Weighted Amp. Sum: %f\n", freqAmpSum);
        Serial.printf("Centroid: %f\n", centroid);
        Serial.printf("==============\n");
    }
}
