#ifndef PedalApp_hpp
#define PedalApp_hpp

using defaultCallback = void (*)(float* output, float* input, unsigned bufferSize,
                              unsigned samplingRate, unsigned numChannelsOut,
                              unsigned numChannelsIn,double streamTime, 
                              PedalApp* app);
PedalApp* createApp(defaultCallback* audioCallback);
void startAudioThread(PedalApp* app);
bool runApp(PedalApp* app);
void updateApp(PedalApp* app);
void deleteApp(PedalApp* app);
#endif 