#include "Formants.h"

// copy references to stored data
void FormantProfile::set_profile(float* freqs /*, float *bandwidth*/)
{
    frequency = freqs;
    // bandwidth = bandwidth;

    // if frequency normalizaiton is disabled, return early
    if (FREQUENCY_NORMALIZATION) {
        // normalize frequencies by dividing by highest frequency
        for (int i = 0; i < 5; i++) {
            frequency[i] /= frequency[4];
            // bandwidth[i] /= frequency[4];
        }
    }
}

Formants::Formants()
{
    this->addSubmodule(&peak_finder);
}

char vowel_to_character(int vowel)
{
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

float Formants::calculate_distance(FormantProfile* profile, float** found_peaks)
{
    // initialize distance to 0
    // a partial sum is added to distance each iteration of the for loop
    float distance = 0;

    // calculate partial sum for 5 peaks
    for (int fpeak = 0; fpeak < NUM_FPEAKS; fpeak++) {
        // calculate distance between measured peak and profile peak
        // 1. take the absolute difference in frequency between peaks
        float d = abs(profile->frequency[fpeak] - found_peaks[0][fpeak]);
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

    for (int i = 0; i < 5; i++) {
        Serial.printf("%f ", found_peaks[0][i]);
    }
    Serial.printf("\n");

    // initalize variables used to save the best match
    float lowest_distance = FLT_MAX;
    int   vocal_register  = -1;
    int   formant_vowel   = -1;

    // compare the f peaks against stored formant profiles
    for (int reg = BASS; reg < SOPRANO + 1; reg++) {
        for (int vow = VOWEL_A; vow < VOWEL_U + 1; vow++) {

            // calculate distance
            float distance = calculate_distance(&formant_table[reg][vow], found_peaks);

            // if distance is lowest seen so far, update best match information
            if (distance < lowest_distance) {
                lowest_distance = distance;
                vocal_register  = reg;
                formant_vowel   = vow;
            }
        }
    }

    // output the character of the best matched vowel
    output = vowel_to_character(formant_vowel);
    Serial.printf("%c\n", output);
}
