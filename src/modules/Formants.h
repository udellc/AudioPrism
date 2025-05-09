#ifndef Formants_h
#define Formants_h

#include <float.h>
#include <math.h>

#include "../AnalysisModule.h"
#include "MajorPeaks.h"

// Frequency Normalization (true/false):
// Enabled: Compare fpeaks by relative frequency
//      -- makes matching indifferent to the root note being sung
//      -- all lists of frequencies get divided by their highest frequency member
//      -- example: {100, 200, 400, 800, 1500} -> {0.066, 0.133, 0.266, 0.533, 1}
// Disabled: Compare fpeaks by absolute frequency
#define FREQUENCY_NORMALIZATION false

// Number of fpeaks to match against (MUST BE 2-5, 3+ RECOMMENDED):
// The first fpeaks are typically more important
// Lower NUM_FPEAK values may make matching more robust to noise
#define NUM_FPEAKS 3

#define FPEAK_PENALTY 0.9

//============================================================================
// HELPERS
//============================================================================

// These macros are used to make indexing the formant table more readable:
// The formant table stores frequency, amplitude, and bandwidth data for each vowel in each vocal register

// vocal register macros
#define BASS    0
#define TENOR   1
#define CTENOR  2
#define ALTO    3
#define SOPRANO 3

// vowel macros
#define VOWEL_A 0
#define VOWEL_E 1
#define VOWEL_I 2
#define VOWEL_O 3
#define VOWEL_U 4

class FormantProfile {
public:
    float* frequency;
    // float *bandwidth;

    // copy references to stored data
    void set_profile(float* freqs /*, float *bandwidth*/)
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
};

//============================================================================
// FORMANT DATA TABLE
//============================================================================

// Data from the Department of Computer Science at the University of Chicago
// https://www.classes.cs.uchicago.edu/archive/1999/spring/CS295/Computing_Resources/Csound/CsManual3.48b1.HTML/Appendices/table3.html

//============================================================================
// CLASS DEFINITION
//============================================================================
class Formants : public ModuleInterface<char> {
private:
    MajorPeaks peak_finder = MajorPeaks(5);

public:
    FormantProfile formant_table[5][5];

    Formants()
    {
        this->addSubmodule(&peak_finder);
        for (int i = 0; i < 5; i++) {
            vocals.formant_table[i][0].set_profile(bass[i]);
            vocals.formant_table[i][1].set_profile(tenor[i]);
            vocals.formant_table[i][2].set_profile(ctenor[i]);
            vocals.formant_table[i][3].set_profile(alto[i]);
            vocals.formant_table[i][4].set_profile(soprano[i]);
        }
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

    float calculate_distance(FormantProfile* profile, float** found_peaks)
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

    void doAnalysis()
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
        output = vowel_to_character(formant_vowel);
        Serial.printf("%c\n", output);
    }
};

#endif
