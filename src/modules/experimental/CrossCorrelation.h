#ifndef CrossCorrelation_h
#define CrossCorrelation_h

#include <cstdint>
/*
 * class for computing correlation coefficient between two signals
 */
class CrossCorrelation
{
    private:
        uint16_t *templatePtr;          ///< pointer to buffer containing template data

        uint32_t templateSqrtSumSq;     ///< stores the square root of the sum squared of template

        uint16_t numRows;               ///< number of rows in template
        uint16_t numCols;               ///< number of columns in template

        uint16_t sampleRate;            ///< sample rate of template
        uint16_t windowSize;            ///< window size of template
        float frequencyWidth;           ///< value to use for template values scaling
        uint16_t frequencyIndexLow;     ///< bin index corresponding to lowest frequency
        uint16_t frequencyIndexHigh;    ///< bin index corresponding to highest frequency 

        /**
         * preliminary computations of template data for correlation
         */
        void computeTemplate(void);
    
    public:
        /**
         * Default constructor
         */
        CrossCorrelation();

        /**
         * constructor for CrossCorrelation
         * @param sampleRate sample rate of signal
         * @param windowSize window size of signal
         */
        CrossCorrelation(uint16_t sampleRate, uint16_t windowSize);

        /**
         * set some buffer as template for cross correlation
         * @param input a pointer to some buffer containing template data
         * @param numRows number of rows in data
         * @param numCols number of columns in data
         * @param frequencyRangeLow start frequency for correlation
         * @param frequencyRangeHigh end frequency for correlation
         */
        void setTemplate(uint16_t *input, uint16_t numRows, uint16_t numCols, uint16_t frequencyRangeLow, uint16_t frequencyRangeHigh);

        /**
         * computes correlation coefficient between template and input signal
         * @param input a pointer to buffer containing data
         * @param inputLatestWindowIndex column at which to start correlation
         * @param inputTotalWindows total columns in input signal
         */
        float correlate(uint16_t *input, uint16_t inputLatestWindowIndex, uint16_t inputTotalWindows);
};

#endif