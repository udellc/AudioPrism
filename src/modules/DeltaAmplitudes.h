//============================================================================
// MODULE INFORMATION
//============================================================================
// Name        : DeltaAmplitudes
// Return Type : float* (list of ampltidue deltas, indexed by frequency bin)
// Description : Used to find the change in amplitudes between the current and
//               previous FFT window for each bin.
//============================================================================

#ifndef Delta_Amplitudes_h
#define Delta_Amplitudes_h

#include "../AnalysisModule.h"
#include <cmath>

// DeltaAmplitudes inherits from the ModuleInterface with a float* output type
class DeltaAmplitudes : public ModuleInterface<float*> {
public:
    float* deltaAmplitudes;

    DeltaAmplitudes();

    ~DeltaAmplitudes();

    void doAnalysis();

    void printOutput();
};

#endif
