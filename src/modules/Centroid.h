//============================================================================
// MODULE INFORMATION
//============================================================================
// Name        : Centroid
// Return Type : int (center of mass of the frequency spectrum)
// Description : Analysis method that calculates the "center of mass" of the
//               frequency spectrum. The output is calculated by summing the
//               product of the frequency and amplitude of each bin and
//               dividing that sum by the total amplitude of the spectrum.
//               The output of this module can be interpreted as a measure
//               of the brightness of the input audio.
//============================================================================

#ifndef Centroid_h
#define Centroid_h

#include "../AnalysisModule.h"
#include <cmath>

// Centroid inherits from the ModuleInterface with an int output type
class Centroid : public ModuleInterface<float> {
public:
    float centroid;
    int   freqResBy2 = freqRes / 2; // divide freqRes by 2 to get the center value

    void doAnalysis();
};

#endif
