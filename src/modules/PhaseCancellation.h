//============================================================================
// MODULE INFORMATION
//============================================================================
// Name        : PhaseCancellation
// Return Type : float 
// Description : TODO: CREATE MODULE WHEN STEREO IS ADDED 
// What is Phase Cancellation: 
// - Phase cancallation is a phenomenon that occurs when a waveform and the same 
//   waveform but with different (multiplied by -1) starting positions are played together. 
//   This causes the waves to "cancel" each other out, leaving no sound. 
// - The purpose of this module is to use this phenomenon to isolate sounds, 
//   such as vocals, so we can get a clearer 
// 
// Sources to look at:   
// - https://www.iconcollective.edu/remove-vocals-from-songs?          
// - https://www.adsrsounds.com/mixing-tutorials/phase-cancellation/?srsltid=AfmBOooY0C_0Gm7DTXUbqoAZZXPqs1OTMX0_d4QbTfhDOkIATImhXcxT
//    this one has a video example of how it actually works in production
//  
//============================================================================

#ifndef PhaseCancellation_h
#define PhaseCancellation_h

/* 
    class PhaseCancellation : public ModuleInterface<float**> {
    private:
        // create constructor and destructor here
        PhaseCancellation() 
        {
            output = new float*[2];
        }

        ~PhaseCancellation() 
        {
            delete[] output;
        }

    public:
        void doAnalysis(const float** input, int numSamples){
            // define a frequency range of sound that you want (band-pass filter)
            // apply phase cancellation to other frequencies 

            // Two methods for how to do actual phase cancellation - for doAnalysis
            // - invert one stereo channel 
            // - multiply the waveform by -1 in the code 
            // sum the two together

            Probably do something like this: 
            for (int i = 0; i < numSamples; ++i) {
                output[i] = input[0][i] + (-1 * input[1][i]);
                // could also do: input[0][i] - input[1][i]
            }

            // could try to add band-pass filter here or could set that in the ino
            // can try to further isolate vocals by applying phase cancellation
            // to other frequencies within the range as well 
        }

        // for debug printing 
        void PrintOutput() 
        {
            for (int i = 0; i < numSamples; ++i) {
                Serial.println(output[i]); 
            }
        }

    }

*/

#endif
