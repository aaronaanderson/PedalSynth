#include "PedalApp.hpp"
#include <iostream>

#define _USE_MATH_DEFINES
#include <cmath>

#include "pedal/WTSaw.hpp"
#include "pedal/utilities.hpp"
#include "pedal/MIDIEvent.hpp"
#include "pedal/CREnvelope.hpp"

WTSaw saw;
SmoothValue<float> frequency;
CREnvelope envelope;
MIDIEvent midiEvent;

void midiCallback(double deltaTime, std::vector<unsigned char>* message, PedalApp* app){
  MIDIEvent event(message);
  switch(event.getEventType()){
    case MIDIEvent::EventTypes::NOTE_ON:
    frequency.setTarget(mtof(event.getNoteNumber()));
    envelope.setTrigger(true);
    //std::cout << "Note On | " << event.getNoteNumber() << " " << event.getNoteVelocity() << std::endl;
    break;
    case MIDIEvent::EventTypes::NOTE_OFF:
    //std::cout << "Note Off | " << event.getNoteNumber() << " " << event.getNoteVelocity() << std::endl;
    envelope.setTrigger(false);
    break;
  }
}
void audioCallback(float* output,float* input, unsigned bufferSize, unsigned samplingRate, unsigned outputChannels,
              unsigned inputChannels, double time, PedalApp* app) {
  frequency.setTime(appGetSlider(app, 0));
  for(int i = 0; i < bufferSize; i++){
    saw.setFrequency(frequency.process());
    float currentSample = saw.generateSample();
    currentSample *= envelope.generateSample();

    for(int j = 0; j < outputChannels; j++){
      output[i * outputChannels + j] = currentSample * 0.1f;
    }
  }
}
int main(){
  PedalApp* app = createApp(audioCallback, midiCallback);
  appAddSlider(app, 0, "Portamento", 0.0f, 2000.0f, 500.0f);
  //appAddSlider(app, 1, "Second Slider", 0.0f, 1.0f, 0.7f);
  //appAddToggle(app, 0, "toggle", false);
  //appAddTrigger(app, 0, "tigger");
  
  openMidiPort(app, 1);
  startAudioThread(app);
  while(runApp(app)){
    updateApp(app);
  }
  deleteApp(app);
}