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
#include "../SpectralTools.h"

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
    PercussionDetection(float flux_threshold, float energy_threshold, float entropy_threshold);

    // set the delta threshold
    // delta is the change in amplitude from the previous window to the current one
    // setting this threshold defines how much a transient must spike in volume to be detected
    void setFluxThreshold(float new_threshold);

    // set the loudness threshold
    // loudness is the immediate total amplitude of the input
    // setting this threshold defines how loud a transient must be to be detected
    void setEnergyThreshold(float new_threshold);

    // set the noisiness threshold (0-1)
    // noisiness is the immediate spectral entropy of the input
    // setting this threshold distinguishes percussive from tonal transients
    void setEntropyThreshold(float new_threshold);

    void doAnalysis();
};

#endif // PERCUSSION_DETECTION_H
