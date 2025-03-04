//============================================================================
// MODULE INFORMATION
//============================================================================
// Name        : PercussionDetection
// Return Type : bool (percussion detected in the current window)
// Description : This module is used to detect the presence of noisy
//               transients. It uses the TotalAmplitude, DeltaAmplitude, and
//               Noisiness modules to determine if a percussive sound is
//               present. The thresholds for each of these modules can be set
//               by the user to better fit the specific qualities of the input
//               signal.
//----------------------------------------------------------------------------
// TIPS FOR USE: Choosing the proper thresholds for TotalAmplitude, Delta-
//               Amplitude, and Noisiness is crucial for accurate percussion
//               detection, becasue this module can be sensitive to the
//               particular mixing qualities of the input signal.
//
//               Setting low threshold will improve the detection of
//               percussion, but may also increase the number of false
//               positives.
//
//               Setting thresholds higher will decrease the number of false
//               positives, but may also decrease the number of true positives.
//
//               Setting a threshold to 0 will effectively disable the
//               corresponding submodule.
//
//               In general, limiting the frequency range of this module to
//               only the higher frequencies of the input signal will improve
//               detection accuracy. Mid and low frequencies are often
//               cluttered with periodic elements, which can obscure
//               percussion or trigger false positives.
//============================================================================

#ifndef Percussion_Detection_h
#define Percussion_Detection_h

#include "../AnalysisModule.h"
#include "DeltaAmplitudes.h"
#include "Noisiness.h"
#include "TotalAmplitude.h"

// TODO: enable delta analysis when input buffer is fixed

// PercussionDetection inherits from the ModuleInterface with a bool output type
// this submodule contains three submodules:
// Submodule 1: TotalAmplitude -- an indicator of the overall loudness of the current window
// Submodule 2: DeltaAmplitude -- an indicator of the change in loudness between the current and previous windows
//              (percussion is expected to have a high delta amplitude compared to sustained audio)
// Submodule 3: Noisiness -- an indicator of the noisiness of the current window
//              (percussion is expected to be noisier than periodic instruments like synths, pianos, strings, etc.)
class PercussionDetection : public ModuleInterface<bool> {
private:
    // submodules are made private so they cannot be accessed outside of the parent module

    // the TotalAmplitude submodule is taken as an indicator of the overall loudness of the input signal
    // the output of this submodule is compared against loudness_threshold
    // percussion is assumed to be louder than most sustained musical elements
    TotalAmplitude totalAmp = TotalAmplitude();

    // the loudness threshold is the minimum amplitude required for a window to be considered percussive
    float loudness_threshold = 300.0;

    // the DeltaAmplitude submodule outputs the change in loudness between the current and previous windows
    // the output of this submodule is compared against delta_threshold
    // percussion is characterized by a sharp increase in loudness, so a high delta amplitude is expected
    DeltaAmplitudes deltaAmp = DeltaAmplitudes();

    // the delta threshold is the minimum change in amplitude required for a window to be considered percussive
    // by default, the delta threshold is correlated with the loudness threshold by a multiplicative factor 0.8
    float delta_threshold = 0.5 * loudness_threshold;

    // the Noisiness submodule gives an indication of how evenly spread energy is across the frequency spectrum
    // the output of this submodule is compared against noise_threshold
    // percussion is expected to be noisy, having energy spread evenly across the audio spectrum
    Noisiness noise = Noisiness();

    // the noise threshold is the minimum noisiness required for a window to be considered percussive
    float noise_threshold = 0.7; // possible range: 0.0 to 1.0

public:
    // default constructor
    // a constructor is necessary for modules containing submodules
    // all submodules must be registered with the parent in the constructor
    PercussionDetection()
    {
        // register submodules
        this->addSubmodule(&totalAmp); // register the TotalAmplitude submodule
        this->addSubmodule(&deltaAmp); // register the DeltaAmplitude submodule
        this->addSubmodule(&noise); // register the Noisiness submodule
    }

    // constructor with parameters for the threshold values
    // this funciton sets the threshold values and register the submodules
    PercussionDetection(float loudness_threshold, float delta_threshold, float noise_threshold)
    {
        // set the threshold values
        this->setLoudnessThreshold(loudness_threshold);
        this->setDeltaThreshold(delta_threshold);
        this->setNoisinessThreshold(noise_threshold);

        // register submodules
        this->addSubmodule(&totalAmp); // register the TotalAmplitude submodule
        this->addSubmodule(&deltaAmp); // register the DeltaAmplitude submodule
        this->addSubmodule(&noise); // register the Noisiness submodule
    }

    // set the loudness threshold
    // loudness is the immediate total amplitude of the input
    // setting this threshold defines how loud a transient must be to be detected
    void setLoudnessThreshold(float new_threshold)
    {
        if (new_threshold < 0) {
            loudness_threshold = 0;
        } else
            loudness_threshold = new_threshold;
    }

    // set the delta threshold
    // delta is the change in amplitude from the previous window to the current one
    // setting this threshold defines how much a transient must spike in volume to be detected
    void setDeltaThreshold(float new_threshold)
    {
        if (new_threshold < 0) {
            delta_threshold = 0;
        } else
            delta_threshold = new_threshold;
    }

    // set the noisiness threshold (0-1)
    // noisiness is the immediate spectral entropy of the input
    // setting this threshold distinguishes percussive from tonal transients
    void setNoisinessThreshold(float new_threshold)
    {
        if (new_threshold < 0) {
            noise_threshold = 0;
        } else if (new_threshold > 1) {
            noise_threshold = 1;
        } else
            noise_threshold = new_threshold;
    }

    void doAnalysis(const float** input)
    {
        // run the submodules' analysses
        totalAmp.doAnalysis(input);
        deltaAmp.doAnalysis(input);
        noise.doAnalysis(input);

        // retrieve the outputs of the submodules
        float total = totalAmp.getOutput();
        float* delta = deltaAmp.getOutput();
        float noiseOutput = noise.getOutput();

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
};
#endif
