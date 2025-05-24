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
#define NUM_FPEAKS 5

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
    float* frequency;
    // float *bandwidth;

    // copy references to stored data
    void set_profile(float* freqs /*, float *bandwidth*/);
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
    // The number of formant peaks to use in the analysis (between 2-5)
    int num_fpeaks = 5;

    MajorPeaks peak_finder = MajorPeaks(5);

    float bass[5][5]    = { { 600.0, 1040.0, 2250.0, 2450.0, 2750.0 }, { 400, 1620, 2400, 2800, 3100 }, { 250, 1750, 2600, 3050, 3340 }, { 400, 750, 2400, 2600, 2900 }, { 350, 600, 2400, 2675, 2900 } };
    float tenor[5][5]   = { { 650, 1080, 2650, 2900, 3250 }, { 400, 1700, 2600, 3200, 3580 }, { 290, 1870, 2800, 3250, 3540 }, { 400, 800, 2600, 2800, 3000 }, { 350, 600, 2700, 2900, 3300 } };
    float ctenor[5][5]  = { { 660, 1120, 2750, 3000, 3350 }, { 440, 1800, 2700, 3000, 3350 }, { 270, 1850, 2900, 3350, 3590 }, { 430, 820, 2700, 3000, 3300 }, { 370, 630, 2750, 3000, 3400 } };
    float alto[5][5]    = { { 800, 1150, 2800, 3500, 4950 }, { 400, 1600, 2700, 3300, 4950 }, { 350, 1700, 2700, 3700, 4950 }, { 450, 800, 2830, 3500, 4950 }, { 325, 700, 2530, 3500, 4950 } };
    float soprano[5][5] = { { 800, 1150, 2900, 3900, 4950 }, { 350, 200, 2800, 3600, 4950 }, { 270, 2140, 2950, 3900, 4950 }, { 450, 800, 2830, 3800, 4950 }, { 325, 700, 2700, 3800, 4950 } };

public:
    FormantProfile formant_table[5][5];

    Formants();

    char vowel_to_character(int vowel, float distance);

    float calculate_distance(FormantProfile* profile, float* found_peaks);

    int interpolateAroundPeak(const float* data, int indexOfPeak, int sampleRate, int windowSize);

    void setNumPeaks(int peaks);

    void doAnalysis();
};

#endif
