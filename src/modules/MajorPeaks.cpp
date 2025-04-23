#include "MajorPeaks.h"

void MajorPeaks::doAnalysis()
{
    resetPeaksArrays();
    findPeaks();
    trimPeaks();
    storePeaks();

    // if debug is enabled, print the output to the serial console
    if (debugMode & DEBUG_ENABLE) {
        Serial.printf("===MAJORPEAKS===\n");
        if (debugMode & DEBUG_VERBOSE) {
            printModuleInfo();
        }
        printOutput();
        Serial.printf("================\n");
    }
}
