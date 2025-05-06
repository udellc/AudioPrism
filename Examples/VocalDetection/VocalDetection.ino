#include <AudioLab.h>
#include <arduinoFFT.h>
#include <VibrosonicsAPI.h>
#include <AudioPrism.h>

// arduino FFT stuff...
float vReal[WINDOW_SIZE];
float vImag[WINDOW_SIZE];
ArduinoFFT<float> FFT = ArduinoFFT<float>(vReal, vImag, WINDOW_SIZE, SAMPLE_RATE);

// get pointer to AudioLab input buffer on channel 0
int* AudioLabInputBuffer = AudioLab.getInputBuffer(0);

float inputBuffer[8][129];
Spectrogram<float> buffer = Spectrogram((float*)inputBuffer, 8, 129);

Formants vocals = Formants();

void setup() {
    Serial.begin(115200);
    while (!Serial)
        ;
    delay(1000);

    // init AudioLab
    AudioLab.init();
}

void loop() {
    // AudioLab.ready() returns true when synthesis should occur/input buffer fills (this returns true at (SAMPLE_RATE / WINDOW_SIZE) times per second)
    if (AudioLab.ready()) {

        // copy samples from AudioLab input buffer to vReal array, set vImag to 0
        for (int i = 0; i < WINDOW_SIZE; i++) {
            vReal[i] = AudioLabInputBuffer[i];
            vImag[i] = 0.0;
        }

        // do arduinoFFT stuff...
        FFT.dcRemoval();
        FFT.windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
        FFT.compute(FFT_FORWARD);
        FFT.complexToMagnitude();

        float tempbuffer[129];
        for(int i = 0; i < 129; i++){
            tempbuffer[i] = vReal[i];
        }
        buffer.pushWindow(tempbuffer);

        const float* rebuiltBuffer[8];
        for(int i = -7; i < 1; i++){
            rebuiltBuffer[i + 7] = buffer.getWindow(i);
        }

        vocals.doAnalysis(rebuiltBuffer);
        Serial.printf("%c\n", vocals.getOutput());
    }
}