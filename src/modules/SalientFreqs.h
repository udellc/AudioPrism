//============================================================================
// MODULE INFORMATION
//============================================================================
// Name        : SalientFreqs
// Return Type : int* (array of freq. bin indexes with the highest amplitude delta)
// Description : SalientFreqs is used to find the n (numFreqs) bin indexes
//               with the highest change in amplitude. The output is an array
//               of bin indexes, sorted by the magnitude of the change in
//               amplitude.
//============================================================================

#ifndef Salient_Freqs_h
#define Salient_Freqs_h

#include "../AnalysisModule.h"
#include "DeltaAmplitudes.h"

// SalientFreqs inherits from the ModuleInterface with an int* output type
class SalientFreqs : public ModuleInterface<int*> {
public:
    int             numFreqs;
    int*            salientFreqs;
    float*          amplitudes;
    DeltaAmplitudes deltaAmps = DeltaAmplitudes();

    SalientFreqs();

    SalientFreqs(int n);

    ~SalientFreqs();

    // change the amount of salient frequencies to be found
    void changeNumFreqs(int newSize);

    // finds the n (numFreqs) bins with highest change in amplitude, stored in salientFreqs[]
    // input is assumed to be a 2D array of FFT data passed in from the Vibrosonics class
    void doAnalysis();

    void printOutput();
};

#endif
