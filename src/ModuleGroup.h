#ifndef MODULE_GROUP_H
#define MODULE_GROUP_H

#include <vector>

#include "AnalysisModule.h"
#include "Spectrogram.h"

class ModuleGroup {
public:
    ModuleGroup(Spectrogram<float>* spectrogram);

    ModuleGroup(Spectrogram<float>* spectrogram, int lowerFreq, int upperFreq);

    void addModule(AnalysisModule* module);

    void addModule(AnalysisModule* module, int lowerFreq, int upperFreq);

    void addModule(AnalysisModule* module, Spectrogram<float>* spectrogram, int lowerFreq, int upperFreq);

    void setSpectrogram(Spectrogram<float>* Spectrogram);

    void runAnalysis();

private:
    int lowerFreq = 0;
    int upperFreq = SAMPLE_RATE >> 1;
    Spectrogram<float>* spectrogram;
    std::vector<AnalysisModule*> modules;
};

#endif // MODULE_GROUP_H
