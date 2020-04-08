
Create a folder called PedalSynth

Download the necessary repositories

https://github.com/glfw/glfw
https://github.com/ocornut/imgui
https://github.com/thestk/rtaudio
https://github.com/thestk/rtmidi
https://github.com/aaronaanderson/Pedal

Place these folders in the PedalSynth folder created earlier

Create two files called PedalApp.hpp and PedalApp.cpp in the PedalSynth folder
In PedalApp.hpp:
```cpp
#ifndef PedalApp_hpp
#define PedalApp_hpp

#endif
```
In PedalApp.cpp:
```cpp
#include PedalApp.hpp
```
These files will contain a PedalApp class that takes care of the library interface. It is where we will define window size, window behavior, samplerate, buffer size, etc. 
### Tell CMake what to do
Create a file called CMakeLists.txt
The CMakeLists.txt tells CMake how to generate our generators (make or ninja files). With the CMakeLists.txt, we can make platform dependent build rules. 
In CMakeLists.txt
```CMake 
cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
```
We should make a file to hold the main program. Make the file PedalSynth.cpp and place it in the PedalSynth folder. 
In PedalSynth:
```cpp
#include PedalApp.hpp
#include <iostream>
int main(){
  std::cout << "Hello World" << std::endl;
}
```
If making a repo, make a readme.md 
in readme.md:
```md
# PedalSynth
## A synth app made with pedal
```

If making a repository (you should), now is a good time. The most simple approach is to make a new repository online with the same name, clone it to your computer, copy in all the files we just created, and run the command
``` 
git add .
```
It is important to be careful with [git add .] This is the only time I use it. If this were done after using CMake, all the CMake files will be part of the repository and tracked. Do not make this mistake, it is painful to undo. 

The first place to start now that we have the necessary files is the CMakeLists.txt. To do a minimum build, we must use CMakeLists to compile the PedalApp.cpp and give access to PedalSynth.cpp.
In CMakeLists.txt
```CMake
cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

add_library(pedal_app STATIC
  PedalApp.cpp
)

add_executable(PedalSynth PedalSynth.cpp)
target_link_libraries(PedalSynth PRIVATE pedal_app)
```
We should now be able to use CMake to build generators, and use the generator to compile that application.

In the PedalSynth folder, create a folder called build. Move to this folder in terminal. 

execute command:
```
cmake ../../PedalSynth
```
this should generate the files, then type command:
``` 
make
```
This should compile the program so far. To run the program, enter
```
./PedalSynth
```
If succesful, Hello World should have printed to the console. 

It makes sense to start next with GLFW for window creation. 
In CMakeLists.txt after the add_library() command:
```CMake
#GLFW, RTAudio, and IMGUI portions from Kee's work on pedal
# Build all dependencies as static libraries
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
if(MSVC)
    # suppress general warning on C functions such as strcpy, etc.
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# glfw for window creation
set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW Documentation" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "Installation Target" FORCE)
add_subdirectory(glfw)

target_link_libraries(pedal_app PUBLIC glfw)
```
In these steps we gave instructions on how to build/include glfw, and added this target to pedal_synth. pedal_synth is added to the executable, so we are now free to use it. 

Now we can add GLFW to the PedalApp class
In PedalApp.hpp
```cpp
struct PedalApp;
void initialize(PedalApp* app);
bool run(PedalApp* app);
void updateApp(PedalApp* app);
void deleteApp(PedalApp* app);
```
These functions will allow us to control the application from PedalSynth.cpp. The PedalApp is not defined here. This is to allow it to compile without having to include everything every time and increase the build time. I got this trick from Kee Youn. Now we should define the PedalApp struct and the three functions in the cpp.
In PedalApp.cpp
```cpp
#include "PedalApp.hpp"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include <atomic>
#include <iostream>

struct PedalApp{
  GLFWwindow* window;//application window
  std::atomic<float> cursorx;
  std::atomic<float> cursory;
};

//==============This is needed for glfw window keyboard input callback
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if ((action == GLFW_PRESS) && (mods & GLFW_MOD_CONTROL) && (key == GLFW_KEY_Q)) {
      glfwSetWindowShouldClose(window, 1);
  }
}

PedalApp* createApp(){
  PedalApp* app = new PedalApp;
  if(!app){
    std::cout << "error initializing app" << std::endl;
  }
  //Make Window
  if (!glfwInit()) {
    std::cerr << "Fail: glfwInit\n";
    delete app;
    return nullptr;
  }
  glfwDefaultWindowHints();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);  // if OSX, this is a must
  glfwWindowHint(GLFW_AUTO_ICONIFY, false);  // so fullcreen does not iconify
  app->window = glfwCreateWindow(640, 480, "Pedal Synth", nullptr, nullptr);
  if (!app->window) {
    std::cerr << "Fail: glfwCreateWindow\n";
    glfwTerminate();
    delete app;
    return nullptr;
  }
  glfwSetKeyCallback(app->window, keyCallback);
  glfwMakeContextCurrent(app->window);
  glfwSwapInterval(1);

  return app;
}
bool runApp(PedalApp* app){
  glfwSwapBuffers(app->window);
  glfwPollEvents();
  return glfwWindowShouldClose(app->window) ? false : true;
}
static float clampf01(float x) {
    if (x < 0.0f) return 0.0f;
    else if (x > 1.0f) return 1.0f;
    else return x;
}
void updateApp(PedalApp* app){
  int display_w, display_h;
  glfwGetFramebufferSize(app->window, &display_w, &display_h);
  int window_w, window_h;
  glfwGetWindowSize(app->window, &window_w, &window_h);
  double cursorx, cursory;
  glfwGetCursorPos(app->window, &cursorx, &cursory);
  
  float cx = (float)(cursorx / window_w);
  float cy = (float)(cursory / window_h);
  cx = clampf01(cx);
  cy = clampf01(cy);
  app->cursorx.store(cx);
  app->cursory.store(cy);
}
void deleteApp(PedalApp* app){
  glfwDestroyWindow(app->window);
  glfwTerminate();
  delete app;
}
```
Now we need to add this functions to PedalSynth.cpp
In PedalSynth.cpp
```cpp
int main(){
  PedalApp* app = createApp();
  while(runApp(app)){
    updateApp(app);
  }
  deleteApp(app);
  std::cout << "hello world" << std::endl;
}
```
Now when we compile the program, a blank window should pop up with the title "Pedal Synth". Closing the window terminates the program by causing runApp to return false. Note that the keyCallback callback function must be defined before the createApp function. This allows keyboard input; it is currently being used to allow closing the app by pressing ctl+q.

Now that we can run the program indefinitely, we are equipped to add RTAudio. First we should build the library and give PedalApp access to it.
In CMakeLists.txt
```CMake
# Rtaudio for audio IO
set(RTAUDIO_BUILD_STATIC_LIBS ON CACHE BOOL "Rtaudio Shared Lib" FORCE)
set(RTAUDIO_BUILD_TESTING OFF CACHE BOOL "Rtaudio Testing" FORCE)
set(RTAUDIO_TARGETNAME_UNINSTALL
    RTAUDIO_UNINSTALL CACHE STRING "Rtaudio Uninstall Target" FORCE)
add_subdirectory(rtaudio)

target_include_directories(pedal_app PUBLIC rtaudio)
target_link_libraries(pedal_app PUBLIC glfw rtaudio)

add_executable(PedalSynth PedalSynth.cpp)
target_link_libraries(PedalSynth PRIVATE pedal_app)
```
Compiling will now include RTSound, and we have access to it in PedalApp

We will have to create a temporary definition of an audio callback function in the header in order to create a function that initializes the audio thread.
In PedalApp.hpp
```cpp
using tempCallback = void (*)(float* output, float* input, unsigned bufferSize,
                              unsigned samplingRate, unsigned numChannelsOut,
                              unsigned numChannelsIn,double streamTime, 
                              PedalApp* app);
PedalApp* createApp(tempCallback* audioCallback);
void startAudioThread(PedalApp* app);
```
tempCallback is a function pointer. It will be defined again in the cpp since it will then have access to RTAudio. We are trying to avoid including that here.
The following code blocks will all belong to PedalSynth.cpp.
Include RTAudio
```cpp
#include "RtAudio.h"

struct PedalApp{
  GLFWWindow* window;
  RTAudio rtaudio;
  std::string device_name;
  unsigned device_id;
  unsigned input_channels;
  unsigned output_channels;
  unsigned sampling_rate;
  unsigned buffer_size;
  tempCallback callback;
  std::atomic<float> cursorx;
  std::atmoic<float> cursory;
}
```
Define a new audio callback before the createApp(tempCallback) function
```cpp
static int audioCallback(void *outputBuffer, void *inputBuffer,
                         unsigned int nFrames, double streamTime,
                         RtAudioStreamStatus status, void *userData) {
    float* out = (float*)outputBuffer;
    float* in = (float*)inputBuffer;

    auto* app = (pdlExampleApp*)userData;
    if (app && app->callback) {
        app->callback(out, in, nFrames, app->sampling_rate, app->output_channels,
                      app->input_channels, streamTime, app);
    }
    return 0;
}
```
Add the following to the function createApp(tempCallback) after the glfw code
```cpp
  app->callback = callback;
  unsigned default_out = app->audio.getDefaultOutputDevice();
  auto device_info = app->audio.getDeviceInfo(default_out);
  app->device_id = default_out;
  app->device_name = device_info.name;
  app->input_channels = device_info.inputChannels;
  app->output_channels = device_info.outputChannels;
  app->sampling_rate = device_info.preferredSampleRate;
  app->buffer_size = 512;

  RtAudio::StreamParameters outputParameters;
  outputParameters.deviceId = app->device_id;
  outputParameters.nChannels = app->output_channels;
  outputParameters.firstChannel = 0;
  RtAudio::StreamParameters inputParameters;
  inputParameters.deviceId = app->device_id;
  inputParameters.nChannels = app->input_channels;
  inputParameters.firstChannel = 0;
  try {
    app->audio.openStream(&outputParameters, &inputParameters, RTAUDIO_FLOAT32,
                          app->sampling_rate, &app->buffer_size,
                          callback, app,
                          nullptr, nullptr); // option & error callback
  }
  catch (RtAudioError& e) {
    e.printMessage();
    glfwDestroyWindow(app->window);
    glfwTerminate();
    delete app;
    return nullptr;
  }
```
Here we have interfaced with rtaudio. Note that I assigned the input device to whatever the output device is, but this need not be the case. 
Now we can define the startAudioThread(PedalApp* app) function
```cpp
void startAudioThread(PedalApp* app){
  app->rtaudio.startStream();
}
```
Add the following to the top of deleteApp(PedalApp* app)
```cpp
try {
  app->rtaudio.stopStream();
}
catch (RtAudioError& e) {
  e.printMessage();
}
if (app->rtaudio.isStreamOpen()) {
  app->rtaudio.closeStream();
}
```
We should now be able to define and use an (identical) audio callback function in PedalSynth.cpp
In PedalSynth.cpp before int main()
```cpp
void callback(float* out,float* in, unsigned bufferSize, unsigned samplingRate, unsigned outputChannels,
              unsigned inputChannels, double time, pdlExampleApp* app) {

}
```
the int main() function should now look like this
```cpp
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
```
Now we have a blank screen app with an audio callback function for input and output. We can test the audio callback by implementing a trivial sine wave. 
```cpp
#define _USE_MATH_DEFINES
#include <cmath>
float phase = 0.0f;
double phaseIncrement = (M_PI * 2.0f * 440.0f)/48000.0f;
void callback(float* out,float* in, unsigned bufferSize, unsigned samplingRate, unsigned outputChannels,
              unsigned inputChannels, double time, pdlExampleApp* app) {
  float currentSample = std::sin(phase) * 0.1f;
  phase += phaseIncrement;
}
```
Now when the application runs a 440Hz sine wave should sound. This is the hello world of real time audio programming. 
