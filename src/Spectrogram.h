/*
 * @file
 * Contains the Spectrogram class definition.
 */

#ifndef SPECTROGRAM_H
#define SPECTROGRAM_H

#include <cstdint>
#include <cstring>

/**
 * Spectrogram holds the frequency domain data over multiple time windows.
 *
 * This class implements a circular buffer to store multiple windows of
 * frequency domain data. Pushing new windows of data will overwrite the oldest
 * window stored. It requires the buffer data to be allocated by the user.
 */
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
     * with the specified number of frequency bins in each window. Each window
     * will contain a number of frequency bins equal half the WINDOW_SIZE.
     * Initializes the current index to 0.
     *
     * @param numWindows The number of time windows the Spectrogram holds.
     */
    Spectrogram(const uint16_t numWindows);

    ~Spectrogram();

    float* getBuffer() const { return this->buffer; };

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
    float* getWindowAt(int relativeIndex) const;

    /**
     * Gets the most recent Spectrogram window data.
     *
     * @return Array of frequency domain data
     */
    float* getCurrentWindow() const;

    /**
     * Gets the previous Spectrogram window data.
     *
     * @return Array of frequency domain data
     */
    float* getPreviousWindow() const;

    /**
     * Pushes a new window to the Spectrogram.
     *
     * Takes new window data and adds it to the circular buffer If the buffer
     * index reaches the maximum column index, it wraps around and overwrites
     * the oldest data, setting the index back to 0
     *
     * @param data Pointer to the window's frequency domain data.
     */
    void pushWindow(const float* data);

    /**
     * Clears the Spectrogram's data buffer and resets the current index.
     */
    void clearBuffer();

private:
    float*   buffer;
    uint16_t numWindows;
    uint16_t numBins;
    uint16_t currIndex;
};

#endif // SPECTROGRAM_H
