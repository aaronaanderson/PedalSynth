#ifndef PedalApp_hpp
#define PedalApp_hpp

struct PedalApp;
PedalApp* createApp();
bool runApp(PedalApp* app);
void updateApp(PedalApp* app);
void deleteApp(PedalApp* app);
#endif 