#ifndef Amplitude_Tools_h
#define Amplitude_Tools_h 

#include "../AnalysisModule.h"
#include <cmath>
#include <vector>

class AmplitudeTools {
    public:
    //Sum of Amplitudes in lowerBinBound, upperBinBound
    static float total(const float** input, int lowerBinBound, int upperBinBound) {
        float total = 0.0f;
        for (int i = lowerBinBound; i < upperBinBound; ++i) {
            total += input[CURR_WINDOW][i];
        } 
        return total;
    }

    //Mean of amplitudes in lowerBinBound, upperBinBound
    static float mean(const float** input, int lowerBinBound, int upperBinBound) {
        float t = total(input, lowerBinBound, upperBinBound);
        return t / float(upperBinBound - lowerBinBound);
    }

    //Max amplitude in lowerBinBound, upperBinBound
    static float max(const float **input, int lowerBinBound, int upperBinBound) {
        float maxVal = 0.0f;
        for (int i = lowerBinBound; i < upperBinBound; ++i) {
            if (input[CURR_WINDOW][i] > maxVal) {
                maxVal = input[CURR_WINDOW][i];
            }    
        }
        return maxVal;
    }

    //Sum of absolute delta amplitudes between current and previous window
    static float delta(const float **input, int lowerBinBound, int upperBinBound) {
        float sumDelta = 0.0f
        for (int i = lowerBinBound; i < upperBinBound;++i) {
            sumDelta += std::abs(input[CURR_WINDOW][i] - input[PREV_WINDOW][i]);
        }
        return sumDelta; 
    }

    //Per-bin delta amplitudes between current and previous window
    static std::vector<float> deltaArray(const flaot** input, int lowerBinBound, int upperBinBound) {
        int size = upperBinBound - lowerBinBound;
        std::vector<float> deltas(size);
        for (int i  = lowerBinBound; i < upperBinBound; ++i) {
            deltas[i - lowerBinBound] = std::abs(input[CURR_WINDOW][i] - input[PREV_WINDOW][i]);
        }
        return deltas;
    }
}; 

#endif //AMPLITUDE_TOOLS_H
