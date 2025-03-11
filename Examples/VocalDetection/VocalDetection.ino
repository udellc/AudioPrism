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

float bass[5][5] = {{600.0, 1040.0, 2250.0, 2450.0, 2750.0}, {400, 1620, 2400, 2800, 3100}, {250, 1750, 2600, 3050, 3340}, {400, 750, 2400, 2600, 2900}, {350, 600, 2400, 2675, 2900}};
float tenor[5][5] = {{650, 1080, 2650, 2900, 3250}, {400, 1700, 2600, 3200, 3580}, {290, 1870, 2800, 3250, 3540}, {400, 800, 2600, 2800, 3000}, {350, 600, 2700, 2900, 3300}};
float ctenor[5][5] = {{660, 1120, 2750, 3000, 3350}, {440, 1800, 2700, 3000, 3350}, {270, 1850, 2900, 3350, 3590}, {430, 820, 2700, 3000, 3300}, {370, 630, 2750, 3000, 3400}};
float alto[5][5] = {{800, 1150, 2800, 3500, 4950}, {400, 1600, 2700, 3300, 4950}, {350, 1700, 2700, 3700, 4950}, {450, 800, 2830, 3500, 4950}, {325, 700, 2530, 3500, 4950}};
float soprano[5][5] = {{800, 1150, 2900, 3900, 4950}, {350, 200, 2800, 3600, 4950}, {270, 2140, 2950, 3900, 4950}, {450, 800, 2830, 3800, 4950}, {325, 700, 2700, 3800, 4950}};

void setup() {
    Serial.begin(115200);
    while (!Serial)
        ;
    delay(1000);
    
    for(int i = 0; i<5; i++){
        vocals.formant_table[i][0].set_profile(bass[i]);
        vocals.formant_table[i][1].set_profile(tenor[i]);
        vocals.formant_table[i][2].set_profile(ctenor[i]);
        vocals.formant_table[i][3].set_profile(alto[i]);
        vocals.formant_table[i][4].set_profile(soprano[i]);
    }

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