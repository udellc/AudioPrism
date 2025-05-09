//============================================================================
// MODULE INFORMATION
//============================================================================
// Name        : BreadSlicer
// Return Type : float*
// Description : Analysis method that splits the frequency spectrum into slices,
//               sums the amplitude within those ranges, and uses the sums as
//               weights for a specified list of output frequencies.
//============================================================================
#ifndef Bread_Slicer_h
#define Bread_Slicer_h

#include "../AnalysisModule.h"
#include <cmath>

// BreadSlicer inherits from the ModuleInterface with a float* output type
class BreadSlicer : public ModuleInterface<float*> {
private:
    int* bandIndexes;
    int  numBands;

public:
    // default constructor, initializes private members.
    // For now this is the only constructor so setBands() must be used to setup this module
    BreadSlicer();

    // deconstructor, frees member pointers if memory was allocated
    ~BreadSlicer();

    /* sets the bands ('slices') of this module
      Ex. setBands([0, 200, 500, 2000, 4000], 4);
      Band frequencies must be in ascending order, frequencies must be at least
      freqResolution-Hz apart so bands dont overlap
    */
    void setBands(int* frequencyBands, int numBands);

    // Sums the amplitudes in each frequency band and stores the results in output.
    void doAnalysis();

    // Prints the sums (output) from the breadSlicer to the serial console
    // Can be called manually but will be included automatically when debug mode is enabled
    void printOutput();
};

#endif
