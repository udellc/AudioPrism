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
    MajorPeaks peak_finder = MajorPeaks(5);

public:
    FormantProfile formant_table[5][5];

    Formants();

    char vowel_to_character(int vowel);

    float calculate_distance(FormantProfile* profile, float** found_peaks);

    void doAnalysis();
};

#endif
