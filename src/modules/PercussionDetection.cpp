#include "PercussionDetection.h"

PercussionDetection::PercussionDetection()
{
    // register submodules
    this->addSubmodule(&totalAmp); // register the TotalAmplitude submodule
    this->addSubmodule(&deltaAmp); // register the DeltaAmplitude submodule
    this->addSubmodule(&noise);    // register the Noisiness submodule
}

PercussionDetection::PercussionDetection(float loudness_threshold,
    float delta_threshold, float noise_threshold)
{
    // set the threshold values
    this->setLoudnessThreshold(loudness_threshold);
    this->setDeltaThreshold(delta_threshold);
    this->setNoisinessThreshold(noise_threshold);

    // register submodules
    this->addSubmodule(&totalAmp); // register the TotalAmplitude submodule
    this->addSubmodule(&deltaAmp); // register the DeltaAmplitude submodule
    this->addSubmodule(&noise);    // register the Noisiness submodule
}

void PercussionDetection::setLoudnessThreshold(float new_threshold)
{
    if (new_threshold < 0) {
        loudness_threshold = 0;
    } else
        loudness_threshold = new_threshold;
}

void PercussionDetection::setDeltaThreshold(float new_threshold)
{
    if (new_threshold < 0) {
        delta_threshold = 0;
    } else
        delta_threshold = new_threshold;
}

void PercussionDetection::setNoisinessThreshold(float new_threshold)
{
    if (new_threshold < 0) {
        noise_threshold = 0;
    } else if (new_threshold > 1) {
        noise_threshold = 1;
    } else
        noise_threshold = new_threshold;
}

void PercussionDetection::doAnalysis()
{
    // run the submodules' analysses
    totalAmp.doAnalysis();
    deltaAmp.doAnalysis();
    noise.doAnalysis();

    // retrieve the outputs of the submodules
    float  total       = totalAmp.getOutput();
    float* delta       = deltaAmp.getOutput();
    float  noiseOutput = noise.getOutput();

    // calculate mean delta
    float deltaSum = 0;
    for (int i = lowerBinBound; i < upperBinBound; i++) {
        deltaSum += delta[i];
    }

    // predict percussion is present if all three submodule's outputs are above their threshold values
    // an output of true indicates that percussion is predicted to be present in the current window
    // an output of false indicates that percussion is not predicted to be present in the current window
    // the output of this module can be retrieved by calling getOutput() after analysis
    output = (total >= loudness_threshold) && (deltaSum >= delta_threshold) && (noiseOutput >= noise_threshold);

    // if debug is enabled, print the output to the serial console
    if (debugMode & DEBUG_ENABLE) {
        Serial.printf("===PERCUSSION_DETECTION===\n");
        if (debugMode & DEBUG_VERBOSE) {
            printModuleInfo();
        }
        Serial.printf("Total Threshold: %f\n", loudness_threshold);
        Serial.printf("Total Value: %f\n", total);
        Serial.printf("Noise Threshold: %f\n", noise_threshold);
        Serial.printf("Noise Value: %f\n", noiseOutput);
        Serial.printf("Delta Threshold: %f\n", delta_threshold);
        Serial.printf("Delta Value: %f\n", deltaSum);
        Serial.printf("==========================\n");
    }
}
