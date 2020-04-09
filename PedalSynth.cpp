#include "PedalApp.hpp"
#include <iostream>

#define _USE_MATH_DEFINES
#include <cmath>

#include "WTSaw.hpp"
WTSaw saw;
float frequency;
float phase = 0.0f;
double phaseIncrement;
void callback(float* output,float* input, unsigned bufferSize, unsigned samplingRate, unsigned outputChannels,
              unsigned inputChannels, double time, PedalApp* app) {
  saw.setFrequency(appGetSlider(app, 0));
  for(int i = 0; i < bufferSize; i++){
    float currentSample = saw.generateSample() * 0.1f;
    for(int j = 0; j < outputChannels; j++){
      output[i * outputChannels + j] = currentSample;
    }
  }
}
int main(){
  PedalApp* app = createApp(callback);
  appAddSlider(app, 0, "Frequency", 60.0f, 4000.0f, 300.0f);
  startAudioThread(app);
  while(runApp(app)){
    updateApp(app);
  }
  deleteApp(app);
}