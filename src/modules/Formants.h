#ifndef Formants_h
#define Formants_h

#include <math.h>
#include <float.h>
#include "../AnalysisModule.h"
// #include "MajorPeaks.h"

// Frequency Normalization (true/false):
// Enabled: Compare fpeaks by relative frequency
//      -- makes matching indifferent to the root note being sung
//      -- all lists of frequencies get divided by their highest frequency member
//      -- example: {100, 200, 400, 800, 1500} -> {0.066, 0.133, 0.266, 0.533, 1}
// Disabled: Compare fpeaks by absolute frequency 
#define FREQUENCY_NORMALIZATION true 

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
#define SOPRANO 4

// vowel macros
#define VOWEL_A 0
#define VOWEL_E 1
#define VOWEL_I 2
#define VOWEL_O 3
#define VOWEL_U 4

class FormantProfile {
public:
    float *frequency;
    //float *bandwidth;
    
    // copy references to stored data
    void set_profile(float *freqs/*, float *bandwidth*/) {
        frequency = freqs;
        //bandwidth = bandwidth;

        // if frequency normalizaiton is disabled, return early
        if(FREQUENCY_NORMALIZATION) {
            // normalize frequencies by dividing by highest frequency
            for(int i=0; i<5; i++){
                frequency[i] /= frequency[4];
                //bandwidth[i] /= frequency[4];
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
class Formants : public ModuleInterface<char>
{
private:
    MajorPeaks peak_finder = MajorPeaks(5);

    float bass[5][5] = {{600.0, 1040.0, 2250.0, 2450.0, 2750.0}, {400, 1620, 2400, 2800, 3100}, {250, 1750, 2600, 3050, 3340}, {400, 750, 2400, 2600, 2900}, {350, 600, 2400, 2675, 2900}};
    float tenor[5][5] = {{650, 1080, 2650, 2900, 3250}, {400, 1700, 2600, 3200, 3580}, {290, 1870, 2800, 3250, 3540}, {400, 800, 2600, 2800, 3000}, {350, 600, 2700, 2900, 3300}};
    float ctenor[5][5] = {{660, 1120, 2750, 3000, 3350}, {440, 1800, 2700, 3000, 3350}, {270, 1850, 2900, 3350, 3590}, {430, 820, 2700, 3000, 3300}, {370, 630, 2750, 3000, 3400}};
    float alto[5][5] = {{800, 1150, 2800, 3500, 4950}, {400, 1600, 2700, 3300, 4950}, {350, 1700, 2700, 3700, 4950}, {450, 800, 2830, 3500, 4950}, {325, 700, 2530, 3500, 4950}};
    float soprano[5][5] = {{800, 1150, 2900, 3900, 4950}, {350, 200, 2800, 3600, 4950}, {270, 2140, 2950, 3900, 4950}, {450, 800, 2830, 3800, 4950}, {325, 700, 2700, 3800, 4950}};

public:
    FormantProfile formant_table[5][5];

    Formants () {
        this->addSubmodule(&peak_finder);
        for(int i = 0; i<5; i++){
            vocals.formant_table[i][0].set_profile(bass[i]);
            vocals.formant_table[i][1].set_profile(tenor[i]);
            vocals.formant_table[i][2].set_profile(ctenor[i]);
            vocals.formant_table[i][3].set_profile(alto[i]);
            vocals.formant_table[i][4].set_profile(soprano[i]);
        }
    }

    char vowel_to_character(int vowel) {
        switch(vowel) {
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

    float calculate_distance(FormantProfile *profile, float **found_peaks) {
        // initialize distance to 0
        // a partial sum is added to distance each iteration of the for loop
        float distance = 0;
    
        // calculate partial sum for 5 peaks
        for(int fpeak=0; fpeak<NUM_FPEAKS; fpeak++){
            // calculate distance between measured peak and profile peak
            // 1. take the absolute difference in frequency between peaks
            float d = abs(profile->frequency[fpeak] - found_peaks[0][fpeak]);
            // 2. if difference is within bandwidth, set distance to 0
            //d = std::max((float) 0.0, (float) d - profile->bandwidth[fpeak]);    
            // 3. square the distance
            // if frequency normalization is enabled, increase d to prevent underflows
            if(FREQUENCY_NORMALIZATION) { d *= 1000; }
            d *= d;
            
            for(int i=0; i<fpeak; i++) {
                d *= (float) FPEAK_PENALTY;
            }
            
            // add partial sum to total distance
            distance += d;
        }

        // take square root of the complete sum 
        return (float) pow(distance, 0.5);
    }

    int interpolateAroundPeak(float *data, int indexOfPeak, int sampleRate, int windowSize) {
        float _freqRes = sampleRate * 1.0 / windowSize;
        float prePeak = indexOfPeak == 0 ? 0.0 : data[indexOfPeak - 1];
        float atPeak = data[indexOfPeak];
        float postPeak = indexOfPeak == vapi.windowSizeBy2 ? 0.0 : data[indexOfPeak + 1];
        // summing around the index of maximum amplitude to normalize magnitudeOfChange
        float peakSum = prePeak + atPeak + postPeak;
        // interpolating the direction and magnitude of change, and normalizing from -1.0 to 1.0
        float magnitudeOfChange = ((atPeak + postPeak) - (atPeak + prePeak)) / (peakSum > 0.0 ? peakSum : 1.0);
    
        // return interpolated frequency
        return int(round((float(indexOfPeak) + magnitudeOfChange) * _freqRes));
    }

    void doAnalysis(const float **input) {
        
        // find the f peaks in the data
        peak_finder.doAnalysis(input);
        float **found_peaks = peak_finder.getOutput();

        if(found_peaks[0][1] == 0) {
            output = '-';
            return;
        }

        if(FREQUENCY_NORMALIZATION) {
            int n = 0;
            // find highest frequency
            for(int i=0; i<5; i++) {
                if(found_peaks[0][i] != 0) { n = i; }
                else { break; }
            }
            float divisor = found_peaks[0][n];
            // divide entries by highest frequency
            for(int i=0; i<n+1; i++) {
                found_peaks[0][i] /= divisor;
            }
        }
        int formants[5] = {0, 0, 0, 0, 0};
        for(int i=0; i<5; i++) {
            formants[i] = interpolateAroundPeak(input[CURR_WINDOW], round(int(found_peaks[0][i] * freqWidth)), sampleRate, windowSize);
            Serial.printf("%f ", found_peaks[0][i]);
        }
        Serial.printf("\n");

        // initalize variables used to save the best match
        float lowest_distance = FLT_MAX;
        int vocal_register = -1;
        int formant_vowel = -1;
        
        // compare the f peaks against stored formant profiles
        for(int reg=BASS; reg<SOPRANO+1; reg++) {
            for(int vow=VOWEL_A; vow<VOWEL_U+1; vow++) {
                
                // calculate distance
                float distance = calculate_distance(&formant_table[reg][vow], formants);

                // if distance is lowest seen so far, update best match information
                if(distance < lowest_distance) {
                    lowest_distance = distance;
                    vocal_register = reg;
                    formant_vowel = vow;
                }
            }
        }

        // output the character of the best matched vowel

        output = vowel_to_character(formant_vowel);
    }
};

#endif