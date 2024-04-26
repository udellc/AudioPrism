#ifndef Analysis_Module_h
#define Analysis_Module_h

#include <math.h>
#include <AudioLab.h>

#define CURR_WINDOW 0
#define PREV_WINDOW 1

class AnalysisModule
{
protected:
    // audio context
    int sampleRate = 8192;
    int windowSize = 256;

    // dependent constants
    int windowSizeBy2 = windowSize >> 1;
    float freqRes = float(sampleRate) / float(windowSize);
    float freqWidth = float(windowSize) / float(sampleRate);

    // frequency range (by bin index)
    int lowerBinBound = 0;
    int upperBinBound = windowSizeBy2;

    // reference to submodules (used to automatically propagate parameters)
    int numSubmodules = 0;
    AnalysisModule** submodules;

public:
    // pure virtual function to be implemented by dervied classes
    virtual void doAnalysis(const float** input) = 0;

    // set the window size of the analysis module
    // must be a positive power of 2
    void setWindowSize(int windowSize);
    
    // set the sample rate of the analysis module
    void setSampleRate(int sampleRate);

    // if a module needs submodules, call this function in the parent module's constructor 
    // this is necessary to automatically propagate base class parameters to submodules
    void addSubmodule(AnalysisModule* module);

    // set the frequency range to analyze 
    void setAnalysisRangeByFreq(int lowerFreq, int upperFreq);
    void setAnalysisRangeByBin(int lowerBin, int upperBin);
};

// interface for analysis module templatized components
// interface is necessary: if the AnalysisModule class is templated, you can't put it in arrays
// derived classes should inherit from this interface, not AnalysisModule
template <typename T> class ModuleInterface : public AnalysisModule
{
protected:
    T output; // result of most recent analysis

public:
    T getOutput(){ return output;}

};

// include derived analysis modules so they can be used by including just this file
#include "modules/BreadSlicer.h"
#include "modules/Centroid.h"
#include "modules/DeltaAmplitudes.h"
#include "modules/MajorPeaks.h"
#include "modules/MaxAmplitude.h"
#include "modules/MeanAmplitude.h"
#include "modules/Noisiness.h"
#include "modules/PercussionDetection.h"
#include "modules/SalientFreqs.h"
#include "modules/TotalAmplitude.h"

#include "modules/Formants.h"

#endif