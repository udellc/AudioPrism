#include "PercussionDetection.h"

PercussionDetection::PercussionDetection(float flux_threshold, float energy_threshold, float entropy_threshold)
{
    // set the threshold values
    this->setFluxThreshold(flux_threshold);
    this->setEnergyThreshold(energy_threshold);
    this->setEntropyThreshold(entropy_threshold);
}

void PercussionDetection::setEnergyThreshold(float new_threshold)
{
    if (new_threshold < 0.) {
        energy_threshold = 0.;
    } else if (new_threshold > 1.) {
        energy_threshold = 1.;
    } else {
        energy_threshold = new_threshold;
    }
}

void PercussionDetection::setFluxThreshold(float new_threshold)
{
    if (new_threshold < 0.) {
        flux_threshold = 0.;
    } else if (new_threshold > 1.) {
        flux_threshold = 1.;
    } else {
        flux_threshold = new_threshold;
    }
}

void PercussionDetection::setEntropyThreshold(float new_threshold)
{
    if (new_threshold < 0) {
        entropy_threshold = 0;
    } else if (new_threshold > 1) {
        entropy_threshold = 1;
    } else {
        entropy_threshold = new_threshold;
    }
}

void PercussionDetection::doAnalysis()
{
    float* currWindow = spectrogram->getCurrentWindow();
    float* prevWindow = spectrogram->getPreviousWindow();

    int   lowerFreq = lowerBinBound / freqWidth;
    int   upperFreq = upperBinBound / freqWidth;
    float totalAmp  = AudioPrism::sum(currWindow, lowerFreq, upperFreq);

    float flux    = 0.;
    float energy  = 0.;
    float entropy = 0.;
    for (int i = lowerBinBound; i < upperBinBound; ++i) {
        // calculate flux (only finding an increase in amplitude)
        float pos_diff = currWindow[i] - prevWindow[i];
        if (pos_diff < 0) {
            pos_diff = 0;
        }
        flux += pow(pos_diff, 2);

        // calculate energy
        energy += pow(currWindow[i], 2);

        // calculate entropy
        float p = (currWindow[i] + 1e-12) / (totalAmp + 1e-10);
        entropy += p * -(std::log2(p));
    }

    flux /= (energy + 1e-10);
    entropy /= std::log2(upperBinBound - lowerBinBound);

    // predict percussion is present if all three submodule's outputs are above their threshold values
    // an output of true indicates that percussion is predicted to be present in the current window
    // an output of false indicates that percussion is not predicted to be present in the current window
    // the output of this module can be retrieved by calling getOutput() after analysis
    output = (flux > flux_threshold)
        && (entropy > entropy_threshold) && (energy > energy_threshold);
    //

    // if debug is enabled, print the output to the serial console
    if (debugMode & DEBUG_ENABLE) {
        Serial.printf("===PERCUSSION_DETECTION===\n");
        if (debugMode & DEBUG_VERBOSE) {
            printModuleInfo();
        }
        Serial.printf("Flux:     %f\n", flux);
        Serial.printf("Energy:   %f\n", energy);
        Serial.printf("Entropy:  %f\n", entropy);
        Serial.printf("==========================\n");

        if (output) {
            Serial.printf("!!! Percussion Detected !!!\n");
        }
    }
}
