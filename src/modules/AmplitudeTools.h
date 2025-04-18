#ifndef Amplitude_Tools_h
#define Amplitude_Tools_h 

#include "../AnalysisModule.h"

class AmplitudeTools {
    public:
    //Sum of Amplitudes
    static float total(const float** input, int lowerBinBound, int upperBinBound) {
        float total = 0.0f;
        for (int i = lowerBinBound; i < upperBinBound; ++i) {
            total += input[CURR_WINDOW][i];
        } 
        return total;
    }

    //Mean of amplitudes
    static float mean(const float** input, int lowerBinBound, int upperBinBound) {
        float t = total(input, lowerBinBound, upperBinBound);
        return t / float(upperBinBound - lowerBinBound);
    }

    //Max amplitude
    static float max(const float **input, int lowerBinBound, int upperBinBound) {
        float maxVal = 0.0f;
        for (int i = lowerBinBound; i < upperBinBound; ++i) {
            if (input[CURR_WINDOW][i] > maxVal) {
                maxVal = input[CURR_WINDOW][i];
            }    
        }
        return maxVal;
    }
}; 

#endif
