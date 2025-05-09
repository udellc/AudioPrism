#ifndef SPECTRAL_TOOLS
#define SPECTRAL_TOOLS

#include <cstdlib>
#include <vector>

namespace AudioPrism {

inline float sum(const float* windowData, int lowerBinBound, int upperBinBound)
{
    float sum = 0.0f;
    for (int i = lowerBinBound; i < upperBinBound; ++i) {
        sum += windowData[i];
    }
    return sum;
}

// Mean of amplitudes in lowerBinBound, upperBinBound
inline float mean(const float* windowData, int lowerBinBound, int upperBinBound)
{
    float t = sum(windowData, lowerBinBound, upperBinBound);
    return t / float(upperBinBound - lowerBinBound);
}

// Max amplitude in lowerBinBound, upperBinBound
inline float max(const float* windowData, int lowerBinBound, int upperBinBound)
{
    float maxVal = 0.0f;
    for (int i = lowerBinBound; i < upperBinBound; ++i) {
        if (windowData[i] > maxVal) {
            maxVal = windowData[i];
        }
    }
    return maxVal;
}

// Sum of absolute delta amplitudes between current and previous window
inline float flux(const float* currWindow, const float* prevWindow,
    int lowerBinBound, int upperBinBound)
{
    float sumDelta = 0.0f;
    for (int i = lowerBinBound; i < upperBinBound; ++i) {
        sumDelta += std::abs(currWindow[i] - prevWindow[i]);
    }
    return sumDelta;
}

// Per-bin delta amplitudes between current and previous window
// This one should input the deltas array I think, helper functions should allocate memory
// inline std::vector<float> deltaArray(const float* currWindow,
//     const float* prevWindow, int lowerBinBound, int upperBinBound)
// {
//     int                size = upperBinBound - lowerBinBound;
//     std::vector<float> deltas;
//     for (int i = lowerBinBound; i < upperBinBound; ++i) {
//         deltas[i - lowerBinBound] = std::abs(currWindow[i] - prevWindow[i]);
//     }
//     return deltas;
// }

} // AudioPrism

#endif // SPECTRAL_TOOLS
