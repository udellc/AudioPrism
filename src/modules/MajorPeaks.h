#ifndef MAJOR_PEAKS_H
#define MAJOR_PEAKS_H

#define MP_FREQ 0
#define MP_AMP  1

#include "../AnalysisModule.h"

/**
 * @ingroup AnalysisModules
 *
 * @brief Finds the N largest amplitude peaks in the current window.
 *
 * Ouptuts an array of tuples containing the frequency and amplitude of each
 * peak. If there are fewer than N peaks, the remaining elements in the array
 * are padded with zeros.
 */
class MajorPeaks : public ModuleInterface<float**> {
public:
    // default constructor the sets the number of peaks to 4
    MajorPeaks();

    // constructor with optional parameter to set the number of peaks to find
    MajorPeaks(int n);

    // destructor
    // frees memory allocated for the output arrays and temporary storage
    ~MajorPeaks();

    // perform the 4 step analysis
    // 1. resetPeaksArrays() to clear the temporary storage and output arrays, and reset the number of peaks found
    // 2. findPeaks() to find all peaks in the current window (up to windowSizeBy2>>1 peaks)
    // 3. trimPeaks() to remove the smallest peaks until numPeaks <= maxNumPeaks
    // 4. storePeaks() to copy peaks from temporary storage to the actual output arrays
    // this is the function called by the analysis manager to perform the analysis
    // the output is a 2d array of floats, where output[0] is an array of frequencies and output[1] is an array of amplitudes
    // the output is indexed by peak number, and is always in order of lowest freq peak to highest freq peak
    void doAnalysis();

private:
    int maxNumPeaks = 4; // default number of peaks to find
    int numPeaks    = 0; // number of peaks found this iteration

    // temporary storage for calculating the output
    int    outputLength      = windowSizeBy2 >> 1;
    float* outputFrequencies = new float[windowSizeBy2 >> 1];
    float* outputAmplitudes  = new float[windowSizeBy2 >> 1];

    // reset peaks arrays
    // this function is called at the beginning of each call to perform analysis
    // the number of peaks is reset to zero, and the arrays are zeroed out
    // the arrays must be reset to zero to avoid storing peaks from previous analysis calls
    void resetPeaksArrays();

    // find all peaks in the current window
    // a peak is a freq. bin whose amplitude is greater than its neighbors
    // this function does not limit itself to finding maxNumPeaks peaks
    // if more than maxNumPeaks peaks are found, the smallest peaks are removed with trimPeaks later
    // if fewer than maxNumPeaks peaks are found, the remaining peaks are padded with zeros
    // this function should only be called after resetPeaksArrays()
    void findPeaks();

    // remove the smallest peaks until numPeaks <= maxNumPeaks
    // if there are equal to or fewer than maxNumPeaks peaks, this function does nothing
    // this function is called directly after findPeaks()
    // larger peaks are shifted to the left in the temporary storage arrays to overwrite smaller peaks
    void trimPeaks();

    // storePeaks() copies peaks from temporary storage to the actual output arrays
    // this fuction is called after trimPeaks(), so there can only be up to maxNumPeaks peaks to store
    // if there are fewer than maxNumPeaks peaks, the remaining elements are padded with zeros
    void storePeaks();

    // for demo/debugging purposes
    void printOutput();
};

#endif // MAJOR_PEAKS_H
