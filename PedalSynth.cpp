#include "PedalApp.hpp"
#include <iostream>

#define _USE_MATH_DEFINES
#include <cmath>

float frequency;
float phase = 0.0f;
double phaseIncrement;
void callback(float* output,float* input, unsigned bufferSize, unsigned samplingRate, unsigned outputChannels,
              unsigned inputChannels, double time, PedalApp* app) {
  frequency = appGetSlider(app, 0);//get value from slider
  phaseIncrement = (M_PI * 2.0f * frequency)/41000.0f;
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
  //create a slider
  appAddSlider(app, 0, "Frequency", 60.0f, 4000.0f, 300.0f);
  startAudioThread(app);
  while(runApp(app)){
    updateApp(app);
  }
  deleteApp(app);
}