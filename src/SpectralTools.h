#ifndef SPECTRAL_TOOLS
#define SPECTRAL_TOOLS

#include <cmath>
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

inline float geometric_mean(const float* windowData, int lowerBinBound, int upperBinBound)
{
    float product = 1.0f;
    for (int i = lowerBinBound; i < upperBinBound; ++i) {
        product *= windowData[i];
    }
    return pow(product, 1.0f / float(upperBinBound - lowerBinBound));
}

// Sum of absolute delta amplitudes between current and previous window
inline float flux(const float* currWindow, const float* prevWindow,
    int lowerBinBound, int upperBinBound)
{
    float sumDelta = 0.0f;
    for (int i = lowerBinBound; i < upperBinBound; ++i) {
        sumDelta += abs(currWindow[i] - prevWindow[i]);
    }
    return sumDelta;
}

// https://www.youtube.com/watch?v=2s3aJfRr9gE
inline float entropy(const float* windowData, int lowerBinBound, int upperBinBound)
{
    float entropy  = 0.;
    float totalAmp = sum(windowData, lowerBinBound, upperBinBound) + 1e-10;
    for (int i = lowerBinBound; i < upperBinBound; ++i) {
        // p is the spectral density of the bin, or the percent of the total
        // amplitude it containts. For calculating entropy, it is helpful to
        // think of this as the probability of the bin containing energy.
        float p = windowData[i] / totalAmp;
        // Calculate the entropy addition of each bin:
        //
        // -log(p) is the 'self-information' of the bin. It is a measure of how
        // surprising it is to see energy in that bin. If p is close to 1, then
        // it is not surprising at all to see energy there, but if it is small
        // it this value will be greater, meaning it is very suprising to see
        // energy there. This makes sense as p was calculated as the percent of
        // the total energy that the bin contains.
        //
        // We weight this 'self-information' value with the actual percent of
        // the total amplitude that bin has so that bins with very little energy
        // do not dominate the entropy value.
        entropy += p * -(log2(p));
    }

    // normalize the entropy value by the log2 of the number of bins, which is
    // the maximum possible entropy value.
    return entropy / log2(upperBinBound - lowerBinBound);
}

inline float energy(const float* windowData, int lowerBinBound, int upperBinBound)
{
    float energy = 0.;
    for (int i = lowerBinBound; i < upperBinBound; i++) {
        energy += windowData[i] * windowData[i];
    }
    return energy;
}

} // AudioPrism

#endif // SPECTRAL_TOOLS
