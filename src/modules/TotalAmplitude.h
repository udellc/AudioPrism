//============================================================================
// MODULE INFORMATION
//============================================================================
// Name        : TotalAmplitude
// Return Type : float
// Description : Returns the sum of the amplitudes of the frequency bins in
//               the current window. If a frequency range is specified, the
//               module will only consider the bins within the specified range.
//============================================================================
#ifndef Total_Amplitude_h
#define Total_Amplitude_h

#include "../AnalysisModule.h"

// TotalAmplitude inherits from the ModuleInterface with a float output type
class TotalAmplitude : public ModuleInterface<float> {
public:
    // doAnalysis() is called by the analysis manager
    // it finds the sum of the amplitudes of the bins in the selected frequency range
    // the sum is stored in the module's output variable
    // input is a 2D array that contains the stored FFT history
    void doAnalysis();
};

#endif
