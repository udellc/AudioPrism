//============================================================================
// MODULE INFORMATION
//============================================================================
// Name        : Centroid
// Return Type : int (center of mass of the frequency spectrum)
// Description : Analysis method that calculates the "center of mass" of the
//               frequency spectrum. The output is calculated by summing the
//               product of the frequency and amplitude of each bin and
//               dividing that sum by the total amplitude of the spectrum.
//               The output of this module can be interpreted as a measure
//               of the brightness of the input audio.
//============================================================================

#ifndef Centroid_h
#define Centroid_h

#include "../AnalysisModule.h"
#include <cmath>

// Centroid inherits from the ModuleInterface with an int output type
class Centroid : public ModuleInterface<float> {
public:
    float centroid;
    int freqResBy2 = freqRes / 2; // divide freqRes by 2 to get the center value

    void doAnalysis(const float** input)
    {
        // get the sum of amplitudes and sum of frequencies*amplitudes
        float ampSum = 0; // total amp across bins 
        float freqAmpSum = 0;
        int amp, freq;
        // calculate center frequency of the bin
        // i * freqRes gives lower edge, adding freqResBy2 gives center
        for (int i = lowerBinBound; i < upperBinBound; i++) {
            amp = input[CURR_WINDOW][i];
            freq = (i * freqRes + freqResBy2); // use the center frequency of a bin
            ampSum += amp; // get total amp
            freqAmpSum += freq * amp; // get total freq * amp (weighted sum) 
        }
        // if ampSum 0, set centroid to 0 -- this means no signal 
        // otherwise divide weighted frequency sum by total amplitude -- "center of mass"
        // lower centroid tends to be darker bassier sounds, higher centroids tend to be brighter sounds
        centroid = (ampSum == 0) ? 0 : (freqAmpSum / ampSum);
        output = centroid;

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
};

#endif