#ifndef PedalApp_hpp
#define PedalApp_hpp

struct PedalApp;
using defaultCallback = void (*)(float* output, float* input, unsigned bufferSize,
                              unsigned samplingRate, unsigned numChannelsOut,
                              unsigned numChannelsIn,double streamTime, 
                              PedalApp* app);
PedalApp* createApp(defaultCallback audioCallback);
bool runApp(PedalApp* app);
void updateApp(PedalApp* app);
void deleteApp(PedalApp* app);
void startAudioThread(PedalApp* app);

#endif