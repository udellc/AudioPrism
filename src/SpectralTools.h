#ifndef SPECTRAL_TOOLS_H
#define SPECTRAL_TOOLS_H

#include <cmath>
#include <cstdlib>
#include <vector>

#include "Config.h"
#include "HardwareSerial.h"

namespace AudioPrism {

/**
 * Calculate the total amplitude sum of the spectrum over a frequency range.
 *
 * @param windowData The input spectrum data
 * @param lowerFreq The lower frequency bound to analyze
 * @param upperFreq The upper frequency bound to analyze
 */
inline float sum(const float* windowData, int lowerFreq, int upperFreq)
{
    if (lowerFreq < 0 || upperFreq > SAMPLE_RATE >> 1 || lowerFreq > upperFreq) {
        return -1;
    }

    float freqWidth     = (float)WINDOW_SIZE / (float)SAMPLE_RATE;
    int   lowerBinBound = round(lowerFreq * freqWidth);
    int   upperBinBound = round(upperFreq * freqWidth);

    float sum = 0.0f;
    for (int i = lowerBinBound; i < upperBinBound; ++i) {
        sum += windowData[i];
    }

    return sum;
}

/**
 * Calculate the mean amplitude of the spectrum over a frequency range.
 *
 * @param windowData The input spectrum data
 * @param lowerFreq The lower frequency bound to analyze
 * @param upperFreq The upper frequency bound to analyze
 */
inline float mean(const float* windowData, int lowerFreq, int upperFreq)
{
    if (lowerFreq < 0 || upperFreq > SAMPLE_RATE >> 1 || lowerFreq > upperFreq) {
        return -1;
    }

    float freqWidth     = (float)WINDOW_SIZE / (float)SAMPLE_RATE;
    int   lowerBinBound = round(lowerFreq * freqWidth);
    int   upperBinBound = round(upperFreq * freqWidth);

    float t = sum(windowData, lowerFreq, upperFreq);
    return t / float(upperBinBound - lowerBinBound);
}

/**
 * Find the maximum amplitude in a spectrum over a frequency range.
 *
 * @param windowData The input spectrum data
 * @param lowerFreq The lower frequency bound to analyze
 * @param upperFreq The upper frequency bound to analyze
 */
inline float max(const float* windowData, int lowerFreq, int upperFreq)
{
    if (lowerFreq < 0 || upperFreq > SAMPLE_RATE >> 1 || lowerFreq > upperFreq) {
        return -1;
    }

    float freqWidth     = (float)WINDOW_SIZE / (float)SAMPLE_RATE;
    int   lowerBinBound = round(lowerFreq * freqWidth);
    int   upperBinBound = round(upperFreq * freqWidth);

    float maxVal = 0.0f;
    for (int i = lowerBinBound; i < upperBinBound; ++i) {
        if (windowData[i] > maxVal) {
            maxVal = windowData[i];
        }
    }
    return maxVal;
}

/**
 * Calculate the total energy of the spectrum over the frequency range.
 *
 * Energy is the sum of the square of the amplitude in each bin.
 *
 * @param windowData The input spectrum data
 * @param lowerFreq The lower frequency bound to analyze
 * @param upperFreq The upper frequency bound to analyze
 */
inline float energy(const float* windowData, int lowerFreq, int upperFreq)
{
    if (lowerFreq < 0 || upperFreq > SAMPLE_RATE >> 1 || lowerFreq > upperFreq) {
        return -1;
    }

    float freqWidth     = (float)WINDOW_SIZE / (float)SAMPLE_RATE;
    int   lowerBinBound = round(lowerFreq * freqWidth);
    int   upperBinBound = round(upperFreq * freqWidth);

    float energy = 0.;
    for (int i = lowerBinBound; i < upperBinBound; ++i) {
        energy += windowData[i] * windowData[i];
    }

    return energy;
}

/**
 * Calculate the entropy of the spectrum over a frequency range.
 *
 * Entropy is a measure of the distribution of energy over the spectrum. It is
 * useful to know if a spectrum is periodic or noisy. A measure closer to 0.0
 * indicates the spectrum is more periodic, meaning a few tones or notes
 * dominate the signal. An entropy value closer to 1.0 indicates noisiness,
 * which is a common element of percussive features like snare drums and high
 * hats.
 *
 * To get a better understanding of the calculation, you may find this video
 * helpful: https://www.youtube.com/watch?v=2s3aJfRr9gE
 *
 * @param windowData The input spectrum data
 * @param lowerFreq The lower frequency bound to analyze
 * @param upperFreq The upper frequency bound to analyze
 */
inline float entropy(const float* windowData, int lowerFreq, int upperFreq)
{
    if (lowerFreq < 0 || upperFreq > SAMPLE_RATE >> 1 || lowerFreq > upperFreq) {
        return -1;
    }

    float freqWidth     = (float)WINDOW_SIZE / (float)SAMPLE_RATE;
    int   lowerBinBound = round(lowerFreq * freqWidth);
    int   upperBinBound = round(upperFreq * freqWidth);

    float entropy  = 0.;
    float totalAmp = sum(windowData, lowerBinBound, upperBinBound);
    for (int i = lowerBinBound; i < upperBinBound; ++i) {
        // p is the spectral density of the bin, or the percent of the total
        // amplitude it containts. For calculating entropy, it is helpful to
        // think of this as the probability of the bin containing energy. The
        // small offsets are to avoid division by or log of zero errors.
        float p = (windowData[i] + 1e-12) / (totalAmp + 1e-10);
        //
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

// Per-bin delta amplitudes between current and previous window
// This one should input the deltas array I think, math helper functions shouldn't allocate memory
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

/**
 * Computes the spectral flux of a spectrum over a frequency range.
 *
 * The spectral flux is the sum of energy change in the current spectrum from
 * the previous. It can be useful to normalize the flux value by the energy sum
 * in this frequency range.
 *
 * @param currWindow The current input spectrum data
 * @param prevWindow The previous input spectrum data
 * @param lowerFreq The lower frequency bound to analyze
 * @param upperFreq The upper frequency bound to analyze
 */
inline float flux(const float* currWindow, const float* prevWindow,
    int lowerFreq, int upperFreq)
{
    if (lowerFreq < 0 || upperFreq > SAMPLE_RATE >> 1 || lowerFreq > upperFreq) {
        return -1;
    }

    float freqWidth     = (float)WINDOW_SIZE / (float)SAMPLE_RATE;
    int   lowerBinBound = round(lowerFreq * freqWidth);
    int   upperBinBound = round(upperFreq * freqWidth);

    float flux = 0.0f;
    for (int i = lowerBinBound; i < upperBinBound; ++i) {
        flux += pow(currWindow[i] - prevWindow[i], 2);
    }

    return flux;
}

/**
 * Computes the positive spectral flux of a spectrum over a frequency range.
 *
 * The positive spectral flux is the sum of energy increases in the current
 * spectrum compared to the previous. Positive flux is useful for detecting
 * onsets in the spectrum. It can be useful to normalize the flux value by the
 * energy sum in this frequency range.
 *
 * @param currWindow The current input spectrum data
 * @param prevWindow The previous input spectrum data
 * @param lowerFreq The lower frequency bound to analyze
 * @param upperFreq The upper frequency bound to analyze
 */
inline float positive_flux(const float* currWindow, const float* prevWindow,
    int lowerFreq, int upperFreq)
{
    if (lowerFreq < 0 || upperFreq > SAMPLE_RATE >> 1 || lowerFreq > upperFreq) {
        return -1;
    }

    float freqWidth     = (float)WINDOW_SIZE / (float)SAMPLE_RATE;
    int   lowerBinBound = round(lowerFreq * freqWidth);
    int   upperBinBound = round(upperFreq * freqWidth);

    float flux = 0.0f;
    for (int i = lowerBinBound; i < upperBinBound; ++i) {
        float pos_diff = currWindow[i] - prevWindow[i];
        if (pos_diff < 0) {
            pos_diff = 0;
        }
        flux += pow(pos_diff, 2);
    }

    return flux;
}

/**
 * Computes the negative spectral flux of a spectrum over a frequency range.
 *
 * The negative spectral flux is the sum of energy decreases in the current
 * spectrum compared to the previous. It can be useful to normalize the flux
 * value by the energy sum in this frequency range.
 *
 * @param currWindow The current input spectrum data
 * @param prevWindow The previous input spectrum data
 * @param lowerFreq The lower frequency bound to analyze
 * @param upperFreq The upper frequency bound to analyze
 */
inline float negative_flux(const float* currWindow, const float* prevWindow,
    int lowerFreq, int upperFreq)
{
    if (lowerFreq < 0 || upperFreq > SAMPLE_RATE >> 1 || lowerFreq > upperFreq) {
        return -1;
    }

    float freqWidth     = (float)WINDOW_SIZE / (float)SAMPLE_RATE;
    int   lowerBinBound = round(lowerFreq * freqWidth);
    int   upperBinBound = round(upperFreq * freqWidth);

    float flux = 0.0f;
    for (int i = lowerBinBound; i < upperBinBound; ++i) {
        float neg_diff = currWindow[i] - prevWindow[i];
        if (neg_diff > 0) {
            neg_diff = 0;
        }
        flux += pow(neg_diff, 2);
    }

    return flux;
}

/**
 * Smoothes the window data over time, using the waited sum of:
 * (1 - smoothingFactor) smoothedData + (smoothingFactor) windowData.
 *
 * @param newWindow The array containing new input data.
 * @param windowData Input/Output array for the smoothed spectrum data.
 * @param smoothingFactor The ratio to smooth the old and new data by.
 *
 * commenting this here for now: try doing this over a spectrogram, it would
 * need at least one window to do the same thing, but more could be used to
 * smooth over real data instead of the continuously smoothed data
 */
inline void smooth_window_over_time(const float* windowData, float* smoothedData,
    float smoothingFactor = 0.05)
{
    for (int i = 0; i < (WINDOW_SIZE >> 1); ++i) {
        float new_weight = smoothingFactor * windowData[i];
        float old_weight = (1 - smoothingFactor) * smoothedData[i];
        smoothedData[i]  = new_weight + old_weight;
    }
}

} // AudioPrism

#endif // SPECTRAL_TOOLS_H
