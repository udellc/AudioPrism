#include "VocalDetection.h"

VocalDetection::VocalDetection()
{
    for (int i = 0; i < 5; i++) {
        data[i] = CrossCorrelation(sampleRate, windowSize);
    }
    this->data[0].setTemplate((uint16_t*)a_temp, 8, 129, lowerBinBound, upperBinBound);
    this->data[1].setTemplate((uint16_t*)e_temp, 8, 129, lowerBinBound, upperBinBound);
    this->data[2].setTemplate((uint16_t*)i_temp, 8, 129, lowerBinBound, upperBinBound);
    this->data[3].setTemplate((uint16_t*)o_temp, 8, 129, lowerBinBound, upperBinBound);
    this->data[4].setTemplate((uint16_t*)u_temp, 8, 129, lowerBinBound, upperBinBound);
}

void VocalDetection::doAnalysis()
{
    // converts input into a uint16_t so it is same type as templates
    uint16_t input_magnitudes[8][windowSizeBy2 + 1];
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < windowSizeBy2 + 1; j++) {
            float* windowData      = spectrogram->getWindowAt(-i);
            input_magnitudes[i][j] = windowData[j];
        }
    }
    // correlate each template and change output according to the highest threshold value
    float highest = 0.001;
    output        = -1;
    for (int i = 0; i < 5; i++) {
        float cur = data[i].correlate((uint16_t*)input_magnitudes, 0, 8);
        Serial.printf("%f,", cur);
        if (cur > highest) {
            output  = i;
            highest = cur;
        }
    }
}

void VocalDetection::printTemplate()
{
    Serial.print("{");
    for (int i = 0; i < 9; i++) {
        Serial.print("{");
        for (int j = 0; j < 256; j++) {
            Serial.printf("%d, ", (int)a_temp[i][j]);
        }
        Serial.print("}");
    }
    Serial.print("}");
}
