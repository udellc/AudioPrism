//============================================================================
// MODULE INFORMATION
//============================================================================
// Name        : SalientFreqs
// Return Type : int* (array of freq. bin indexes with the highest amplitude delta)
// Description : SalientFreqs is used to find the n (numFreqs) bin indexes
//               with the highest change in amplitude. The output is an array
//               of bin indexes, sorted based on three directional choices
//               direction = (0 = change, 1 = decrease, 2 = increase)
//============================================================================

#ifndef Salient_Freqs_h
#define Salient_Freqs_h

#include "../AnalysisModule.h"
#include "DeltaAmplitudes.h"

// SalientFreqs inherits from the ModuleInterface with an int* output type
class SalientFreqs : public ModuleInterface<int*> {
public:
    int numFreqs;
    int direction;
    int* salientFreqs;
    float* amplitudes;
    DeltaAmplitudes deltaAmps = DeltaAmplitudes();

    SalientFreqs()
    {
        numFreqs = 3; // default to finding frequency of max change
        direction = 0;
        salientFreqs = new int[numFreqs];
        this->addSubmodule(&deltaAmps);

        for (int i = 0; i < numFreqs; i++) {
            salientFreqs[i] = -1;
        }
    }

    /* Constructor that takes in a value to set the number of max change in amplitudes*/
    SalientFreqs(int n, int dir)
    {
        // If n is less than zero set it to zero so that it doesn't break
        if (n < 0) {
            n = 0;
        }
        // If n is greater than the bin range given by windowSizeBy2 truncate it
        if (n > windowSizeBy2) {
            n = windowSizeBy2;
        }
        // If dir isn't one of the directional choice (0, 1, 2)
        if (dir < 0 || dir > 2) {
            dir = 0;
        }

        direction = dir;

        numFreqs = n;
        salientFreqs = new int[numFreqs];
        this->addSubmodule(&deltaAmps);

        for (int i = 0; i < numFreqs; i++) {
            salientFreqs[i] = -1;
        }
    }

    ~SalientFreqs()
    {
        delete[] salientFreqs;
    }

    // change the amount of salient frequencies to be found
    void changeNumFreqs(int newSize)
    {
        numFreqs = newSize;
        delete[] salientFreqs;
        salientFreqs = new int[numFreqs];
    }

    // change the direction of salient frequencies
    void changeDirction(int dir) {
        direction = dir;
    }

    void checkDirection(const float** input, int idx) {
        if (direction == 1 && input[CURR_WINDOW][idx] > input[PREV_WINDOW][idx])
            return false;
        if (direction == 2 && input[CURR_WINDOW][idx] < input[PREV_WINDOW][idx])
            return false;
        return true
    }

    // finds the n (numFreqs) bins with highest change in amplitude, stored in salientFreqs[]
    // input is assumed to be a 2D array of FFT data passed in from the Vibrosonics class
    void doAnalysis(const float** input)
    {
        deltaAmps.doAnalysis(input);
        amplitudes = deltaAmps.getOutput(); // copy amplitudes

        // iterate through amplitudes to find the maximum(s)
        int currMaxAmp = 0;
        int currMaxAmpIdx = -1;
        for (int i = 0; i < numFreqs; i++) {
            for (int j = lowerBinBound; j < upperBinBound; j++) {
                if (amplitudes[j] > currMaxAmp) {
                    if(checkDirection(input, j)){
                        currMaxAmp = amplitudes[j];
                        currMaxAmpIdx = j;
                    }
                }
            }
            salientFreqs[i] = currMaxAmpIdx; // add the new max amp index to the array
            amplitudes[currMaxAmpIdx] = 0; // set the amp to 0 so it will not be found again
            currMaxAmp = 0; // reset iterators
            currMaxAmpIdx = -1;
        }
        output = salientFreqs;

        // if debug is enabled, print the output to the serial console
        if (debugMode & DEBUG_ENABLE) {
            Serial.printf("===SALIENT_FREQS===\n");
            if (debugMode & DEBUG_VERBOSE) {
                printModuleInfo();
            }
            Serial.printf("Salient Freqs: ");
            for (int i = 0; i < numFreqs; i++) {
                Serial.printf("%d ", salientFreqs[i]);
            }
            Serial.printf("\n==========================\n");
        }
    }

};

#endif
