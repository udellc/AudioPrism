#include "Spectrogram.h"
#include "Config.h"
#include <cstdint>

// make default to two windows, allow resizing
Spectrogram::Spectrogram()
{
    this->buffer     = NULL;
    this->numWindows = 0;
    this->numBins    = 0;
    this->currIndex  = 0;
};

Spectrogram::Spectrogram(const uint16_t numWindows)
{
    uint16_t numBins = WINDOW_SIZE >> 1;
    this->buffer     = new float[numWindows * numBins];
    this->numWindows = numWindows;
    this->numBins    = numBins;
    this->currIndex  = 0;
}

Spectrogram::~Spectrogram()
{
    delete[] this->buffer;
    this->buffer = NULL;
};

float* Spectrogram::getWindowAt(int relativeIndex) const
{
    // ensure that modulus math will work for negative values:
    while (relativeIndex < 0) {
        relativeIndex += this->numWindows;
    }
    uint16_t index = (this->currIndex + relativeIndex) % this->numWindows;
    return this->buffer + (index * this->numBins);
};

float* Spectrogram::getCurrentWindow() const
{
    return this->buffer + (this->currIndex * this->numBins);
};

float* Spectrogram::getPreviousWindow() const
{
    uint16_t prev_index = (this->numWindows + this->currIndex - 1) % this->numWindows;
    return this->buffer + (prev_index * this->numBins);
};

void Spectrogram::pushWindow(const float* data)
{
    this->currIndex++;
    if (this->currIndex == this->numWindows) {
        this->currIndex = 0;
    }

    float* windowBuffer = this->buffer + (this->currIndex * this->numBins);
    memcpy(windowBuffer, data, this->numBins * sizeof(float));
};

void Spectrogram::clearBuffer()
{
    for (int i = 0; i < numWindows * numBins; i++) {
        buffer[i] = 0;
    }
    this->currIndex = 0;
};
