#include "PedalApp.hpp"
#include <iostream>

#define _USE_MATH_DEFINES
#include <cmath>

float phase = 0.0f;
double phaseIncrement = (M_PI * 2.0f * 440.0f)/48000.0f;

void callback(float* output,float* input, unsigned bufferSize, unsigned samplingRate, unsigned outputChannels,
              unsigned inputChannels, double time, PedalApp* app) {

  for(int i = 0; i < bufferSize; i++){
    float currentSample = std::sin(phase) * 0.1f;
    phase += phaseIncrement;
    for(int j = 0; j < outputChannels; j++){
      output[i * outputChannels + j] = currentSample;
    }
  }
}

int main(){
  PedalApp* app = createApp(callback);
  startAudioThread(app);
  while(runApp(app)){
    updateApp(app);
  }
  deleteApp(app);
}