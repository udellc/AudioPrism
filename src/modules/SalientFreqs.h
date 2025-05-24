//============================================================================
// MODULE INFORMATION
//============================================================================
// Name        : SalientFreqs
// Return Type : int* (array of freq. bin indexes with the highest amplitude delta)
// Description : SalientFreqs is used to find the n (numFreqs) bin indexes
//               with the highest change in amplitude. The output is an array
//               of bin indexes, sorted based on three directional choices
//               direction = (0 = change, 1 = decrease, 2 = increase)
//============================================================================

#ifndef Salient_Freqs_h
#define Salient_Freqs_h

#include "../AnalysisModule.h"
#include "DeltaAmplitudes.h"

// SalientFreqs inherits from the ModuleInterface with an int* output type
class SalientFreqs : public ModuleInterface<int*> {
public:
    SalientFreqs();

    SalientFreqs(int n);

    /* Constructor that takes in a value to set the number of max change in amplitudes*/
    SalientFreqs(int n, int dir);

    ~SalientFreqs();

    // change the amount of salient frequencies to be found
    void changeNumFreqs(int newSize);

    // change the direction of salient frequencies
    void changeDirction(int dir);

    bool checkDirection(int idx);

    // finds the n (numFreqs) bins with highest change in amplitude, stored in salientFreqs[]
    // input is assumed to be a 2D array of FFT data passed in from the Vibrosonics class
    void doAnalysis();

private:
    int             numFreqs;
    int             direction;
    int*            salientFreqs;
    float*          amplitudes;
    DeltaAmplitudes deltaAmps = DeltaAmplitudes();
};

#endif
