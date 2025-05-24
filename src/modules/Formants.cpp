#include "Formants.h"

Formants::Formants()
{
    this->addSubmodule(&peak_finder);
    for (int i = 0; i < 5; i++) {
        formant_table[i][0].set_profile(bass[i]);
        formant_table[i][1].set_profile(tenor[i]);
        formant_table[i][2].set_profile(ctenor[i]);
        formant_table[i][3].set_profile(alto[i]);
        formant_table[i][4].set_profile(soprano[i]);
    }
}

char Formants::vowel_to_character(int vowel, float distance)
{
    if (distance > 100)
        return '-';
    switch (vowel) {
    case 0:
        return 'a';
    case 1:
        return 'e';
    case 2:
        return 'i';
    case 3:
        return 'o';
    case 4:
        return 'u';
    default:
        return '-';
    }
}

float Formants::calculate_distance(FormantProfile* profile, float* found_peaks)
{
    // initialize distance to 0
    // a partial sum is added to distance each iteration of the for loop
    float distance = 0;

    // calculate partial sum for 5 peaks
    for (int fpeak = 0; fpeak < num_fpeaks; fpeak++) {
        // calculate distance between measured peak and profile peak
        // 1. take the absolute difference in frequency between peaks

        float d = abs(profile->frequency[fpeak] - found_peaks[fpeak]);
        // 2. if difference is within bandwidth, set distance to 0
        // d = std::max((float) 0.0, (float) d - profile->bandwidth[fpeak]);
        // 3. square the distance
        // if frequency normalization is enabled, increase d to prevent underflows
        if (FREQUENCY_NORMALIZATION) {
            d *= 1000;
        }
        d *= d;

        for (int i = 0; i < fpeak; i++) {
            d *= (float)FPEAK_PENALTY;
        }

        // add partial sum to total distance
        distance += d;
    }

    // take square root of the complete sum
    return (float)pow(distance, 0.5);
}

int Formants::interpolateAroundPeak(const float* data, int indexOfPeak, int sampleRate, int windowSize)
{
    float _freqRes = sampleRate * 1.0 / windowSize;
    float prePeak  = indexOfPeak == 0 ? 0.0 : data[indexOfPeak - 1];
    float atPeak   = data[indexOfPeak];
    float postPeak = indexOfPeak == windowSizeBy2 ? 0.0 : data[indexOfPeak + 1];
    // summing around the index of maximum amplitude to normalize magnitudeOfChange
    float peakSum = prePeak + atPeak + postPeak;
    // interpolating the direction and magnitude of change, and normalizing from -1.0 to 1.0
    float magnitudeOfChange = ((atPeak + postPeak) - (atPeak + prePeak)) / (peakSum > 0.0 ? peakSum : 1.0);

    // return interpolated frequency
    return int(round((float(indexOfPeak) + magnitudeOfChange) * _freqRes));
}

void Formants::setNumPeaks(int peaks)
{
    if (peaks <= 5 && peaks >= 2)
        this->num_fpeaks = peaks;
}

void Formants::doAnalysis()
{

    // find the f peaks in the data
    peak_finder.doAnalysis();
    float** found_peaks = peak_finder.getOutput();

    if (found_peaks[0][1] == 0) {
        output = '-';
        return;
    }

    if (FREQUENCY_NORMALIZATION) {
        int n = 0;
        // find highest frequency
        for (int i = 0; i < 5; i++) {
            if (found_peaks[0][i] != 0) {
                n = i;
            } else {
                break;
            }
        }
        float divisor = found_peaks[0][n];
        // divide entries by highest frequency
        for (int i = 0; i < n + 1; i++) {
            found_peaks[0][i] /= divisor;
        }
    }
    float formants[5] = { 0, 0, 0, 0, 0 };
    for (int i = 0; i < 5; i++) {
        formants[i] = interpolateAroundPeak(spectrogram->getCurrentWindow(),
            round(int(found_peaks[0][i] * freqWidth)), sampleRate, windowSize);
    }

    // initalize variables used to save the best match
    float lowest_distance = FLT_MAX;
    int   vocal_register  = -1;
    int   formant_vowel   = -1;

    // compare the f peaks against stored formant profiles
    for (int reg = BASS; reg < SOPRANO + 1; reg++) {
        for (int vow = VOWEL_A; vow < VOWEL_U + 1; vow++) {

            // calculate distance
            float distance = calculate_distance(&formant_table[reg][vow], formants);

            // if distance is lowest seen so far, update best match information
            if (distance < lowest_distance) {
                lowest_distance = distance;
                vocal_register  = reg;
                formant_vowel   = vow;
            }
        }
    }

    // output the character of the best matched vowel
    output = vowel_to_character(formant_vowel, lowest_distance);
}
