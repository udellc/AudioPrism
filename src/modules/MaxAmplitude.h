//============================================================================
// MODULE INFORMATION
//============================================================================
// Name        : MaxAmplitude
// Return Type : float (amplitude of the freq. bin with the highest amplitude)
// Description : Returns the amplitude of the frequency bin with the highest
//               amplitude in the current window. If a frequency range is
//               specified, the module will only consider the bins within the
//               specified range.
//============================================================================
#ifndef Max_Amplitude_h
#define Max_Amplitude_h

#include "../AnalysisModule.h"

// MaxAmplitude inherits from the ModuleInterface with a float output type
class MaxAmplitude : public ModuleInterface<float> {
public:
    // doAnalysis() is called by the analysis manager
    // it finds the frequency bin with the highest amplitude in the current window
    // the max amplitude is stored in the module's output variable
    void doAnalysis();
};

#endif
