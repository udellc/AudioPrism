#include "AnalysisModule.h"

// This Example assumes FFT output is accessible.
float** FFTOutput;  // dummy pointer

// declare modules
MajorPeaks          mp = MajorPeaks(8);
PercussionDetection pd = PercussionDetection(500, 500, 0.6);
SalientFreqs        sf = SalientFreqs();
BreadSlicer         bs = BreadSlicer();
Centroid            cd = Centroid();
DeltaAmplitudes     da = DeltaAmplitudes();
MaxAmplitude        maxAmp = MaxAmplitude();
TotalAmplitude      ta = TotalAmplitude();
Noisiness           noise = Noisiness();
MeanAmplitude       meanAmp = MeanAmplitude();

void setup() {
  // enable debug mode for each module
  mp.setDebugMode(true);
  pd.setDebugMode(true);
  sf.setDebugMode(true);
  bs.setDebugMode(true);
  cd.setDebugMode(true);
  da.setDebugMode(true);
  maxAmp.setDebugMode(true);
  ta.setDebugMode(true);
  noise.setDebugMode(true);
  meanAmp.setDebugMode(true);

  Serial.printf("Ready\n");
}
void loop() {
    // process input data and produce FFT output

    // run analysis for each module
    mp.doAnalysis(FFTOutput);
    pd.doAnalysis(FFTOutput);
    sf.doAnalysis(FFTOutput);
    bs.doAnalysis(FFTOutput);
    cd.doAnalysis(FFTOutput);
    da.doAnalysis(FFTOutput);
    maxAmp.doAnalysis(FFTOutput);
    ta.doAnalysis(FFTOutput);
    noise.doAnalysis(FFTOutput);
    meanAmp.doAnalysis(FFTOutput);

    // map outputs

}
