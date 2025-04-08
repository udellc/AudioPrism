#include "CrossCorrelation.h"
#include <cstdint>
#include <cmath>

CrossCorrelation::CrossCorrelation(){
    
}

CrossCorrelation::CrossCorrelation(uint16_t sampleRate, uint16_t windowSize) {
    this->templatePtr = NULL;

    this->templateSqrtSumSq = 0;
    this->numRows = 0;
    this->numCols = 0;

    this->sampleRate = sampleRate;
    this->windowSize = windowSize;
    this->frequencyWidth = float(this->windowSize) / this->sampleRate;
    this->frequencyIndexLow = 0;
    this->frequencyIndexHigh = (this->sampleRate >> 1) * frequencyWidth;
}

void CrossCorrelation::computeTemplate() {
    this->templateSqrtSumSq = 0;

    uint32_t _sumSq = 0;
    uint16_t _templateValue = 0;
    uint16_t t, f;

    // computing square root of the squared sum of the template spectrogram
    for (t = 0; t < numCols; t++) {
        for (f = this->frequencyIndexLow; f < this->frequencyIndexHigh; f++) {
            _templateValue = *(this->templatePtr + f + t * this->numRows);
            _sumSq += _templateValue * _templateValue;
        }
    }
    this->templateSqrtSumSq = sqrtl(_sumSq);
}


void CrossCorrelation::setTemplate(uint16_t *input, uint16_t numRows, uint16_t numCols, uint16_t frequencyRangeLow, uint16_t frequencyRangeHigh) {
    this->templatePtr = input;
    this->numRows = numRows;
    this->numCols = numCols;

    this->frequencyIndexLow = floor(frequencyRangeLow * frequencyWidth);
    this->frequencyIndexHigh = ceil(frequencyRangeHigh * frequencyWidth);

    this->computeTemplate();
}

float CrossCorrelation::correlate(uint16_t *input, uint16_t inputLatestWindowIndex, uint16_t inputTotalWindows) {
    uint32_t _inputSqrtSumSq = 0;
    uint16_t _inputValue, _templateValue;

    // cross correlation introduces a delay depending on the length of template, to solve this...
    // subtract length of template from current time index in input
    uint16_t _inputWindowIndex = (inputLatestWindowIndex - this->numCols + inputTotalWindows) % inputTotalWindows;

    uint16_t t, f;

    float _correlationCoefficient = 0.0;

    // computing square root sum squared of input
    uint16_t _tempInputWindowIndex = _inputWindowIndex;
    for (t = 0; t < this->numCols; t++) {

        _tempInputWindowIndex += 1;

        if (_tempInputWindowIndex == inputTotalWindows) _tempInputWindowIndex = 0;

        for (f = this->frequencyIndexLow; f < this->frequencyIndexHigh; f++) {
            _inputValue = *(input + f + _tempInputWindowIndex * this->numRows);
            _inputSqrtSumSq += _inputValue * _inputValue;
        }
    }

    // computing product of square root of sum squared of template and input
    _inputSqrtSumSq = sqrtl(_inputSqrtSumSq) * this->templateSqrtSumSq;
    float _inverseSqrtSumSq = 1.0;
    
    // computing inverse of product (to reduce use of division)
    if (_inputSqrtSumSq > 0) _inverseSqrtSumSq = 1.0 / _inputSqrtSumSq;
    else return _correlationCoefficient;

    // computing dot product and correlation coefficient
    _tempInputWindowIndex = _inputWindowIndex;
    for (t = 0; t < this->numCols; t++) {

        _tempInputWindowIndex += 1;

        if (_tempInputWindowIndex == inputTotalWindows) _tempInputWindowIndex = 0;

        for (f = this->frequencyIndexLow; f < this->frequencyIndexHigh; f++) {
            _inputValue = *(input + f + _tempInputWindowIndex * this->numRows);
            _templateValue = *(this->templatePtr + f + t * this->numRows);
            _correlationCoefficient += _inputValue * _templateValue * _inverseSqrtSumSq;
        }
    }

    return _correlationCoefficient;
}