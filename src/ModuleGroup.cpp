#include "ModuleGroup.h"

ModuleGroup::ModuleGroup(Spectrogram* spectrogram)
{
    this->spectrogram = spectrogram;
}

ModuleGroup::ModuleGroup(Spectrogram* spectrogram, int lowerFreq, int upperFreq)
{
    this->lowerFreq   = lowerFreq;
    this->upperFreq   = upperFreq;
    this->spectrogram = spectrogram;
}

void ModuleGroup::addModule(AnalysisModule* module)
{
    module->setSpectrogram(this->spectrogram);
    module->setAnalysisRangeByFreq(this->lowerFreq, this->upperFreq);

    this->modules.push_back(module);
}

void ModuleGroup::addModule(AnalysisModule* module, int lowerFreq, int upperFreq)
{
    module->setSpectrogram(this->spectrogram);
    module->setAnalysisRangeByFreq(lowerFreq, upperFreq);

    this->modules.push_back(module);
}

void ModuleGroup::runAnalysis()
{
    for (AnalysisModule* module : this->modules) {
        module->doAnalysis();
    }
}
