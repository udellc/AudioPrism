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

#ifndef PERCUSSION_DETECTION_H
#define PERCUSSION_DETECTION_H

#include "../AnalysisModule.h"
#include "HardwareSerial.h"
#include "SpectralTools.h"

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
    // the loudness threshold is the minimum amplitude required for a window to be considered percussive
    float energy_threshold = 1800000.0;

    // the delta threshold is the minimum change in amplitude required for a window to be considered percussive
    // by default, the delta threshold is correlated with the loudness threshold by a multiplicative factor 0.8
    float flux_threshold = 0.5;

    // the noise threshold is the minimum noisiness required for a window to be considered percussive
    float entropy_threshold = 0.75; // possible range: 0.0 to 1.0

public:
    PercussionDetection() { }
    // constructor with parameters for the threshold values
    // this funciton sets the threshold values and register the submodules
    PercussionDetection(float flux_threshold, float energy_threshold, float entropy_threshold)
    {
        // set the threshold values
        this->setFluxThreshold(flux_threshold);
        this->setEnergyThreshold(energy_threshold);
        this->setEntropyThreshold(entropy_threshold);
    }

    // set the delta threshold
    // delta is the change in amplitude from the previous window to the current one
    // setting this threshold defines how much a transient must spike in volume to be detected
    void setFluxThreshold(float new_threshold)
    {
        if (new_threshold < 0.) {
            flux_threshold = 0.;
        } else if (new_threshold > 1.) {
            flux_threshold = 1.;
        } else {
            flux_threshold = new_threshold;
        }
    }

    // set the loudness threshold
    // loudness is the immediate total amplitude of the input
    // setting this threshold defines how loud a transient must be to be detected
    void setEnergyThreshold(float new_threshold)
    {
        if (new_threshold < 0.) {
            energy_threshold = 0.;
        } else if (new_threshold > 1.) {
            energy_threshold = 1.;
        } else {
            energy_threshold = new_threshold;
        }
    }

    // set the noisiness threshold (0-1)
    // noisiness is the immediate spectral entropy of the input
    // setting this threshold distinguishes percussive from tonal transients
    void setEntropyThreshold(float new_threshold)
    {
        if (new_threshold < 0) {
            entropy_threshold = 0;
        } else if (new_threshold > 1) {
            entropy_threshold = 1;
        } else {
            entropy_threshold = new_threshold;
        }
    }

    void doAnalysis()
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
};

#endif // PERCUSSION_DETECTION_H
