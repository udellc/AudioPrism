#ifndef FMSYNTHESIS_H
#define FMSYNTHESIS_H

#include <math.h>
#include <AudioLab.h>  // if AudioLab provides useful audio utilities

// FMSynthesis implements a basic FM synthesis algorithm:
// y(t) = amplitude * sin(2π * carrierPhase + modulationIndex * sin(2π * modulatorPhase))
class FMSynthesis {
private:
    float carrierFrequency;    // frequency of the carrier oscillator (Hz)
    float modulatorFrequency;  // frequency of the modulator oscillator (Hz)
    float modulationIndex;     // modulation index determining modulation depth
    float amplitude;           // output amplitude
    int sampleRate;            // samples per second

    // Phase accumulators for carrier and modulator (range 0.0 - 1.0)
    float phaseCarrier;
    float phaseModulator;

public:
    // Constructor with default parameters
    FMSynthesis(int sampleRate = 44100,
                float carrierFrequency = 440.0f,
                float modulatorFrequency = 220.0f,
                float modulationIndex = 2.0f,
                float amplitude = 1.0f)
      : sampleRate(sampleRate),
        carrierFrequency(carrierFrequency),
        modulatorFrequency(modulatorFrequency),
        modulationIndex(modulationIndex),
        amplitude(amplitude),
        phaseCarrier(0.0f),
        phaseModulator(0.0f)
    {}

    // Setters to allow parameter updates
    void setCarrierFrequency(float fc) { carrierFrequency = fc; }
    void setModulatorFrequency(float fm) { modulatorFrequency = fm; }
    void setModulationIndex(float index) { modulationIndex = index; }
    void setAmplitude(float amp) { amplitude = amp; }
    void setSampleRate(int rate) { sampleRate = rate; }

    // Generates a single FM synthesized sample
    float generateSample() {
        // Calculate modulator value (sine wave)
        float modValue = sinf(2.0f * M_PI * phaseModulator);
        // Generate the FM synthesized sample using the carrier plus modulator influence
        float sample = amplitude * sinf(2.0f * M_PI * phaseCarrier + modulationIndex * modValue);

        // Increment phase accumulators by their normalized frequency increments
        phaseModulator += modulatorFrequency / sampleRate;
        phaseCarrier   += carrierFrequency   / sampleRate;

        // Wrap phases to keep them in the interval [0, 1)
        if (phaseModulator >= 1.0f) phaseModulator -= 1.0f;
        if (phaseCarrier   >= 1.0f) phaseCarrier   -= 1.0f;

        return sample;
    }

    // Fills a provided buffer with numSamples synthesized samples
    void generateBuffer(float* buffer, int numSamples) {
        for (int i = 0; i < numSamples; i++) {
            buffer[i] = generateSample();
        }
    }
};

#endif // FMSYNTHESIS_H
