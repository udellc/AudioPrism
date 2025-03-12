#include "ModuleGroup.h"

ModuleGroup::ModuleGroup(Spectrogram<float>* spectrogram)
{
    this->spectrogram = spectrogram;
}

ModuleGroup::ModuleGroup(Spectrogram<float>* spectrogram, int lowerFreq, int upperFreq)
{
    this->lowerFreq = lowerFreq;
    this->upperFreq = upperFreq;
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

void ModuleGroup::setSpectrogram(Spectrogram<float>* Spectrogram)
{
    this->spectrogram = spectrogram;

    for (AnalysisModule* module : this->modules) {
        module->setSpectrogram(spectrogram);
    }
}

void ModuleGroup::runAnalysis()
{
    for (AnalysisModule* module : this->modules) {
        module->doAnalysis();
    }
}
