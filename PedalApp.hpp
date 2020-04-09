#ifndef PedalApp_hpp
#define PedalApp_hpp

struct PedalApp;
using defaultCallback = void (*)(float* output, float* input, unsigned bufferSize,
                              unsigned samplingRate, unsigned numChannelsOut,
                              unsigned numChannelsIn,double streamTime, 
                              PedalApp* app);
#include <vector>
using defaultMidiInputCallback = void (*)(double deltatime, 
                                   std::vector< unsigned char >* message,
                                   PedalApp* app);
PedalApp* createApp(defaultCallback audioCallback, defaultMidiInputCallback midiInputCallback);
bool runApp(PedalApp* app);
void updateApp(PedalApp* app);
void deleteApp(PedalApp* app);
void startAudioThread(PedalApp* app);

void appGetCursorPos(PedalApp* app, float* mx, float* my);
void appAddSlider(PedalApp* app, int sliderIndex, const char* name,
                  float low, float high, float initialValue);
float appGetSlider(PedalApp* app, int idx);
void appAddToggle(PedalApp* app, int toggleIndex, const char* name,
                  bool initialValue);
bool appGetToggle(PedalApp* app, int idx);
void appAddTrigger(PedalApp* app, int triggerIndex, const char* name);
bool appGetTrigger(PedalApp* app, int idx);
void appAddDropDown(PedalApp* app, int idx, const char* name,  
                    char*  content[], int length);
int appGetDropDown(PedalApp* app, int indx);
#endif