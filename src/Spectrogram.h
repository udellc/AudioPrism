/**
 * @file
 * Contains the Spectrogram class definition.
 */

#ifndef SPECTROGRAM_H
#define SPECTROGRAM_H

#include <cstddef>
#include <cstdint>
#include <cstring>

/**
 * Spectrogram holds the frequency domain data over multiple time windows.
 *
 * This class implements a circular buffer to store multiple windows of
 * frequency domain data. Pushing new windows of data will overwrite the oldest
 * window stored. It requires the buffer data to be allocated by the user.
 */
template <typename T>
class Spectrogram {
public:
    /**
     * Creates an empty Spectrogram.
     *
     * Sets all values to 0 or NULL, allocates no memory for the data buffer.
     */
    Spectrogram();

    /**
     * Creates a Spectrogram with a buffer to hold data.
     *
     * Allocates a buffer to hold data for the specified number of windows,
     * with the specified number of frequency bins in each window. Initializes
     * the current index to 0.
     *
     * @param numWindows The number of time windows the Spectrogram holds.
     * @param numBins The number of frequency bins in each window.
     */
    Spectrogram(const uint16_t numWindows, const uint16_t numBins);

    ~Spectrogram();

    /**
     * Set the Spectrogram's data buffer.
     *
     * Updates the Spectrogram's data buffer, number of windows, and number of
     * bins. Allows for the user to allocate their own memory for the
     * Spectrogram's buffer.
     *
     * @param buffer Pointer to the Spectrogram's data buffer.
     * @param numWindows The number of time windows the Spectrogram holds.
     * @param numBin The number of frequency bins in each window.
     */
    void setBuffer(T* buffer, const uint16_t numWindows, const uint16_t numBins);

    T* getBuffer() const { return this->buffer; };

    uint16_t getNumBins() const { return this->numBins; };

    uint16_t getNumWindows() const { return this->numWindows; };

    uint16_t getCurrentIndex() const { return this->currIndex; };

    /**
     * Gets the window data at an index relative to the current.
     *
     * A negative relative index searches backwards in time, a positive index
     * wraps around to the 'tail' of the Spectrogram.
     *
     * @param relativeIndex Index relative to the current.
     * @return Array of frequency domain data
     */
    T* getWindowAt(int relativeIndex) const;

    /**
     * Gets the most recent Spectrogram window data.
     *
     * @return Array of frequency domain data
     */
    T* getCurrentWindow() const;

    /**
     * Gets the previous Spectrogram window data.
     *
     * @return Array of frequency domain data
     */
    T* getPreviousWindow() const;

    /**
     * Pushes a new window to the Spectrogram.
     *
     * Takes new window data and adds it to the circular buffer If the buffer
     * index reaches the maximum column index, it wraps around and overwrites
     * the oldest data, setting the index back to 0
     *
     * @param data Pointer to the window's frequency domain data.
     */
    void pushWindow(const T* data);

    /**
     * Clears the Spectrogram's data buffer and resets the current index.
     */
    void clearBuffer();

private:
    T* buffer;
    uint16_t numWindows;
    uint16_t numBins;
    uint16_t currIndex;
};

template <typename T>
Spectrogram<T>::Spectrogram()
{
    this->buffer = NULL;
    this->numWindows = 0;
    this->numBins = 0;
    this->currIndex = 0;
};

template <typename T>
Spectrogram<T>::Spectrogram(const uint16_t numWindows, const uint16_t numBins)
{
    this->buffer = new T[numWindows * numBins];
    this->numWindows = numWindows;
    this->numBins = numBins;
    this->currIndex = 0;
}

template <typename T>
void Spectrogram<T>::setBuffer(T* buffer, const uint16_t numWindows, const uint16_t numBins)
{
    this->buffer = buffer;
    this->numWindows = numWindows;
    this->numBins = numBins;
};

template <typename T>
Spectrogram<T>::~Spectrogram()
{
    delete[] this->buffer;
    this->buffer = NULL;
};

template <typename T>
T* Spectrogram<T>::getWindowAt(int relativeIndex) const
{
    // ensure that modulus math will work for negative values:
    while (relativeIndex < 0) {
        relativeIndex += this->numWindows;
    }
    uint16_t index = (this->currIndex + relativeIndex) % this->numWindows;
    return this->buffer + (index * this->numBins);
};

template <typename T>
T* Spectrogram<T>::getCurrentWindow() const
{
    return this->buffer + (this->currIndex * this->numBins);
};

template <typename T>
T* Spectrogram<T>::getPreviousWindow() const
{
    return this->buffer + ((this->currIndex - 1) * this->numBins);
};

template <typename T>
void Spectrogram<T>::pushWindow(const T* data)
{
    this->currIndex++;
    if (this->currIndex == this->numWindows)
        this->currIndex = 0;

    T* windowBuffer = this->buffer + (this->currIndex * this->numBins);
    memcpy(windowBuffer, data, this->numBins * sizeof(T));
};

template <typename T>
void Spectrogram<T>::clearBuffer()
{
    for (int i = 0; i < numWindows * numBins; i++) {
        buffer[i] = 0;
    }
    this->currIndex = 0;
};

#endif // SPECTROGRAM
