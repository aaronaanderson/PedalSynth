#include "PedalApp.hpp"
#define _USE_MATH_DEFINES//so we can use M_PI
#include <cmath>//so we can use std::sin()
float phase = 0.0f;
double phaseIncrement = (M_PI * 2.0f * 440.0f)/41000.0f;
void callback(float* out,float* in, unsigned bufferSize, 
              unsigned samplingRate, unsigned outputChannels,
              unsigned inputChannels, double time, PedalApp* app) {
  float currentSample = std::sin(phase) * 0.1f;//calculate sin per sample
  phase += phaseIncrement;//increment phase
}
int main(){
  PedalApp* app = pdlInitExampleApp(callback);
  if (!app) {//if app doesn't succesfully allocate
    return 1;//cancel program, return 1
  }
  //begin the audio thread
  startAudioThread(app);
  while (runApp(app)) {
    updateApp(app);//update the app
  }
  //the application has stopped running, 
  deleteApp(app);//free the app from memory
}