/*
 * @file
 * Contains the ModuleGroup class definition.
 */

#ifndef MODULE_GROUP_H
#define MODULE_GROUP_H

#include <vector>
#include "AnalysisModule.h"
#include "Spectrogram.h"

class ModuleGroup {
public:
    /**
     * Create a group of modules that will analyze the entire spectrogram.
     *
     * @param spectrogram A pointer to the spectrogram to analyze.
     */
    ModuleGroup(Spectrogram* spectrogram);

    /**
     * Create a group of modules that will analyze a specific frequency range.
     *
     * The range is specified in Hz, by a lower and upper frequency bound.
     *
     * @param spectrogram A pointer to the spectrogram to analyze.
     * @param lowerFreq The lower frequency bound (Hz).
     * @param upperFreq The upper frequency bound (Hz).
     */
    ModuleGroup(Spectrogram* spectrogram, int lowerFreq, int upperFreq);

    /**
     * Add a module to the group.
     *
     * The added module will inherit all relevant properties from the group.
     *
     * @param module A pointer to the module to add.
     */
    void addModule(AnalysisModule* module);

    /**
     * Add a module to the group, overriding the frequency range.
     *
     * The added module will have its own frequency analysis range, all other
     * relevant roperties will be inherited from the group.
     *
     * @param module A pointer to the module to add.
     * @param lowerFreq The lower frequency bound (Hz).
     * @param upperFreq The upper frequency bound (Hz).
     */
    void addModule(AnalysisModule* module, int lowerFreq, int upperFreq);

    /**
     * Add a module to the group, overriding the frequency range and spectrogram.
     *
     * The added module will have its own frequency analysis range, and analyze
     * a separate spectrogram from the rest of the group.
     *
     * @param module A pointer to the module to add.
     * @param spectrogram A pointer to the spectrogram to analyze.
     * @param lowerFreq The lower frequency bound (Hz).
     * @param upperFreq The upper frequency bound (Hz).
     */
    void addModule(AnalysisModule* module, Spectrogram* spectrogram,
        int lowerFreq, int upperFreq);

    /**
     * Run the analysis function for all modules in the group.
     */
    void runAnalysis();

private:
    int lowerFreq = 0;
    int upperFreq = SAMPLE_RATE >> 1;

    Spectrogram* spectrogram;

    std::vector<AnalysisModule*> modules;
};

#endif // MODULE_GROUP_H
