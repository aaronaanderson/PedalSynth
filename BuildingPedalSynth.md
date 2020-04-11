
## Building an audio application with glfw, imgui, RtAudio, pedal, and RtMidi
This file documents the process of creating this application.We will use GLFW for the app window management, IMGUI for a GUI, RTAudio for input/output callbacks and device management, Pedal for a sound library, and RTMidi for midi input. We will make two documents, PedalApp.hpp/cpp, to handle the application functionality. We will be using CMake to build this project. I will be using the names PedalApp and PedalSynth, but you may replace these with whatever.

### Create/download documents
Create a folder called PedalSynth

Download the necessary repositories

https://github.com/glfw/glfw

https://github.com/ocornut/imgui

https://github.com/thestk/rtaudio

https://github.com/thestk/rtmidi

https://github.com/aaronaanderson/Pedal

Place these folders in the PedalSynth folder (remove the '-master' in the name)

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
Create a file called CMakeLists.txt in the PedalSynth folder
The CMakeLists.txt tells CMake how to generate our generators (make or ninja files). With the CMakeLists.txt, we can make platform dependent build rules. 
In CMakeLists.txt
```CMake 
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(PedalSynth)
```
We should make a file to hold the main program. Make the file PedalSynth.cpp and place it in the PedalSynth folder. 
In PedalSynth:
```cpp
#include PedalApp.hpp
#include <iostream>//for cout/endl
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
We have added at least a small amount of code to each new document. git add won't add blankd documents. 

It is important to be careful with [git add .] This is the only time I use it. If this were done after using CMake, all the CMake files will be part of the repository and tracked. Do not make this mistake, it is painful to undo. 
Calling:
```
git push origin master
```
should update your online repo.
### Building an application with CMake
The first place to start now that we have the necessary files is the CMakeLists.txt. To do a minimum build, we must use CMakeLists to compile the PedalApp.cpp and give access to PedalSynth.cpp.
In CMakeLists.txt
```CMake
cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(PedalSynth)

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
## Adding GLFW
It makes sense to start next with GLFW for window creation. This is a basic application window with a minimize/fullscreen/exit buttons, resizeable window,window title, etc. 

In CMakeLists.txt after the add_library() command:
```CMake
cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(PedalSynth)

add_library(pedal_app STATIC
  PedalApp.cpp
)

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
add_executable(PedalSynth PedalSynth.cpp)
target_link_libraries(PedalSynth PRIVATE pedal_app)
```
In these steps we gave instructions on how to build/include glfw, and added this target to pedal_synth. pedal_synth is added to the executable, so we are now free to use it. 

Now we can add GLFW to the PedalApp class
In PedalApp.hpp
```cpp
struct PedalApp;
PedalApp* createApp();
bool runApp(PedalApp* app);
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

struct PedalApp{//stores all application data
  GLFWwindow* window;//application window
  std::atomic<float> cursorx;
  std::atomic<float> cursory;
};

//==============This is needed for glfw window keyboard input callback
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
  if ((action == GLFW_PRESS) && (mods & GLFW_MOD_CONTROL) && (key == GLFW_KEY_Q)){
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
PedalSynth.cpp:
```cpp
#include "PedalApp.hpp"

int main(){
  PedalApp* app = createApp();
  while(runApp(app)){
    updateApp(app);
  }
  deleteApp(app);
}
```
Now when we compile the program, a blank window should pop up with the title "Pedal Synth". Closing the window terminates the program by causing runApp to return false. Note that the keyCallback callback function must be defined before the createApp function. This allows keyboard input; it is currently being used to allow closing the app by pressing ctl+q.

## Adding RtAudio
Now that we can run the program indefinitely, we are equipped to add RTAudio. RTAudio will handle communications with the sound card. We can decide how to implement. We will make a single input/output callback function that requires a pointer to both an input and output buffer.
First we should build RtAudio and get access to it.

CMakeLists.txt
```CMake
cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(PedalSynth)

add_library(pedal_app STATIC
  PedalApp.cpp
)

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

We will have to create a definition of an audio callback function in the header in order to create a function that initializes the audio thread.
In PedalApp.hpp
```cpp
using defaultCallback = void (*)(float* output, float* input, unsigned bufferSize,
                              unsigned samplingRate, unsigned numChannelsOut,
                              unsigned numChannelsIn,double streamTime, 
                              PedalApp* app);
PedalApp* createApp(defaultCallback* audioCallback);
void startAudioThread(PedalApp* app);
bool runApp(PedalApp* app);
void updateApp(PedalApp* app);
void deleteApp(PedalApp* app);
```
Here is the full PedalApp.cpp:
```cpp
#include "PedalApp.hpp"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"
#include <atomic>
#include <iostream>

struct PedalApp{//stores all application data
  GLFWwindow* window;//application window
  RtAudio rtaudio;
  std::string device_name;
  unsigned device_id;
  unsigned input_channels;
  unsigned output_channels;
  unsigned sampling_rate;
  unsigned buffer_size;
  defaultCallback callback;
  std::atomic<float> cursorx;
  std::atomic<float> cursory;
};

//==============This is needed for glfw window keyboard input callback
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
  if ((action == GLFW_PRESS) && (mods & GLFW_MOD_CONTROL) && (key == GLFW_KEY_Q)){
    glfwSetWindowShouldClose(window, 1);
  }
}
static int audioCallback(void *outputBuffer, void *inputBuffer,
                         unsigned int nFrames, double streamTime,
                         RtAudioStreamStatus status, void *userData) {
    float* out = (float*)outputBuffer;
    float* in = (float*)inputBuffer;

    auto* app = (PedalApp*)userData;
    if (app && app->callback) {
        app->callback(out, in, nFrames, app->sampling_rate, app->output_channels,
                      app->input_channels, streamTime, app);
    }
    return 0;
}
PedalApp* createApp(){
  PedalApp* app = new PedalApp;
  if(!app){
    std::cout << "error initializing app" << std::endl;
  }
  //==============GLFW
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
  
  //===========RTAudio
  app->callback = callback;
  unsigned default_out = app->rtaudio.getDefaultOutputDevice();
  auto device_info = app->rtaudio.getDeviceInfo(default_out);
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
  return app;
}
void startAudioThread(PedalApp* app){
  app->rtaudio.startStream();
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
  try {
    app->rtaudio.stopStream();
  }
  catch (RtAudioError& e) {
    e.printMessage();
  }
  if (app->rtaudio.isStreamOpen()) {
    app->rtaudio.closeStream();
  }
  glfwDestroyWindow(app->window);
  glfwTerminate();
  delete app;
}
```

The callback functions are a bit odd and call for an explenation. In the header, we had to define a callback function so we could use it as an input for createApp(). However, it was decided to avoid including RtAudio.h in the header to avoid slow compile times. We've created our own callback function in the header that get's called every time RTAudio's callback function get's called. Note we can now give our PedalSynth.cpp file an identical format callback, thus allowing a user to define that function in the CPP. This is demonstrated in the following...
Full PedalSynth.cpp so far

```cpp
#define _USE_MATH_DEFINES//so we can use M_PI
#include <cmath>//so we can use std::sin()
float phase = 0.0f;
double phaseIncrement = (M_PI * 2.0f * 440.0f)/41000.0f;
void callback(float* out,float* in, unsigned bufferSize, 
              unsigned samplingRate, unsigned outputChannels,
              unsigned inputChannels, double time, pdlExampleApp* app) {
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
```
Now when the application runs a 440Hz sine wave should sound. This is the hello world of real time audio programming. 

## Adding IMGUI
Imgui requires that we also  include glew (include with the imgui repository but must be built). The following is the full CMakeLists.txt:
```CMake
cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(PedalSynth)

add_library(pedal_app STATIC
  PedalApp.cpp
)

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

# Rtaudio for audio IO
set(RTAUDIO_BUILD_STATIC_LIBS ON CACHE BOOL "Rtaudio Shared Lib" FORCE)
set(RTAUDIO_BUILD_TESTING OFF CACHE BOOL "Rtaudio Testing" FORCE)
set(RTAUDIO_TARGETNAME_UNINSTALL
    RTAUDIO_UNINSTALL CACHE STRING "Rtaudio Uninstall Target" FORCE)
add_subdirectory(rtaudio)

# imgui for gui
add_library(imgui STATIC imgui/imgui_demo.cpp imgui/imgui_draw.cpp
                         imgui/imgui_widgets.cpp imgui/imgui.cpp)
set_target_properties(imgui PROPERTIES
    DEBUG_POSTFIX d
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    ARCHIVE_OUTPUT_DIRECTORY lib
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG lib
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE lib
)
target_include_directories(imgui PUBLIC imgui)

# gl3w that came with imgui to load GL functions
add_library(gl3w STATIC imgui/examples/libs/gl3w/GL/gl3w.c)
set_target_properties(gl3w PROPERTIES
    DEBUG_POSTFIX d
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    ARCHIVE_OUTPUT_DIRECTORY lib
    ARCHIVE_OUTPUT_DIRECTORY_DEBUG lib
    ARCHIVE_OUTPUT_DIRECTORY_RELEASE lib
)
target_include_directories(gl3w PUBLIC imgui/examples/libs/gl3w)
target_link_libraries(gl3w PUBLIC ${OPENGL_gl_LIBRARY})
target_compile_definitions(gl3w PUBLIC IMGUI_IMPL_OPENGL_LOADER_GL3W)

target_include_directories(pedal_app PUBLIC rtaudio imgui/examples)
target_link_libraries(pedal_app PUBLIC glfw gl3w imgui rtaudio)

add_executable(PedalSynth PedalSynth.cpp)
target_link_libraries(PedalSynth PRIVATE pedal_app)
```
We will only compile and have access to what we need from imgui, which will include dropdown menus, sliders, toggles, and triggers. We need a unique function to create elements and a way to access elements. 
Full PedalApp.hpp so far:
```cpp
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
```
Note that I added appGetCursorPos() here even though we use GLFW for that function, not IMGUI

Now we must add a few includes to the cpp and create structs for each of these GUI elements
Full PedalApp.cpp so far:
```cpp
#include "PedalApp.hpp"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "GL/gl3w.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "RtAudio.h"
#include "RtMidi.h"
#include <string>
#include <atomic>
#include <iostream>

#define NUM_SLIDERS_MAX 16
#define NUM_TOGGLES_MAX 16
#define NUM_TRIGGERS_MAX 16
#define NUM_DROPDOWNS_MAX 16

struct slider {
    std::string name;
    std::atomic<float> atomic_val;
    float low, high;
    float val;
};
struct toggle {
    std::string name;
    std::atomic<bool> atomic_val;
    bool val;
};
struct trigger {
    std::string name;
    std::atomic<bool> atomic_val;
    bool val;
};
struct dropDown{
    std::string name;
    std::atomic<int> atomic_val;
    char** content;
    int length;
    int val;
};

struct PedalApp{
  GLFWwindow* window;//application window
  RtAudio rtaudio;
  std::string device_name;
  unsigned device_id;
  unsigned input_channels;
  unsigned output_channels;
  unsigned sampling_rate;
  unsigned buffer_size;
  defaultCallback callback;
  RtMidiIn* rtMidiIn;
  defaultMidiInputCallback midiInputCallback;
  std::string midiDeviceName;
  unsigned int numPorts;
  slider sliders[NUM_SLIDERS_MAX];
  toggle toggles[NUM_TOGGLES_MAX];
  trigger triggers[NUM_TRIGGERS_MAX];
  dropDown dropDowns[NUM_DROPDOWNS_MAX];
  std::atomic<float> cursorx;
  std::atomic<float> cursory;
};

//==============This is needed for glfw window callback
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if ((action == GLFW_PRESS) && (mods & GLFW_MOD_CONTROL) && (key == GLFW_KEY_Q)) {
      glfwSetWindowShouldClose(window, 1);
  }
}
//==============This will be called by RtAudio. Our function is called inside.
static int audioCallback(void *outputBuffer, void *inputBuffer,
                         unsigned int nFrames, double streamTime,
                         RtAudioStreamStatus status, void *userData) {
    float* out = (float*)outputBuffer;
    float* in = (float*)inputBuffer;

    auto* app = (PedalApp*)userData;
    if (app && app->callback) {
        app->callback(out, in, nFrames, app->sampling_rate, app->output_channels,
                      app->input_channels, streamTime, app);
    }
    return 0;
}
PedalApp* createApp(defaultCallback callback, defaultMidiInputCallback midiCallbackIn){
  PedalApp* app = new PedalApp;
  if(!app){
    std::cout << "error initializing app" << std::endl;
  }
  //==================================GLFW
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

  app->callback = callback;
  unsigned default_out = app->rtaudio.getDefaultOutputDevice();
  auto device_info = app->rtaudio.getDeviceInfo(default_out);
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
  auto* inparams = &inputParameters;
  if (app->input_channels > 0) {
    inputParameters.deviceId = app->device_id;
    inputParameters.nChannels = app->input_channels;
    inputParameters.firstChannel = 0;
  }
  else {
    // cannot open stream with 0 channels so pass nullptr in that case
    inparams = nullptr;
  }
  try {
    app->rtaudio.openStream(&outputParameters,
                            inparams,
                            RTAUDIO_FLOAT32,
                            app->sampling_rate,
                            &app->buffer_size,
                            audioCallback, app,
                            nullptr, // options
                            nullptr); // error callback
  }
  catch (RtAudioError& e) {
    e.printMessage();
    glfwDestroyWindow(app->window);
    glfwTerminate();
    delete app;
    return nullptr;
  }

  //=====================IMGUI
  if (gl3wInit() != 0) {
    std::cerr << "Fail: gl3wInit\n";
    glfwDestroyWindow(app->window);
    glfwTerminate();
    delete app;
    return nullptr;
  }
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(app->window, true);
  ImGui_ImplOpenGL3_Init("#version 330");

  for (int i = 0; i < NUM_SLIDERS_MAX; i += 1) {
    slider* s = app->sliders + i;
    s->name = "";
    s->low = 0.0f;
    s->high = 1.0f;
    s->val = 0.0f;
    s->atomic_val.store(0.0f);
  }
  for (int i = 0; i < NUM_TOGGLES_MAX; i += 1) {
    toggle* t = app->toggles + i;
    t->name = "";
    t->val = false;
    t->atomic_val.store(false);
  }
  for (int i = 0; i < NUM_TRIGGERS_MAX; i += 1) {
    trigger* t = app->triggers + i;
    t->name = "";
    t->val = false;
    t->atomic_val.store(false);
  }
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
  //Update GUI
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGuiWindowFlags flags = 0;
  flags |= ImGuiWindowFlags_NoTitleBar;
  flags |= ImGuiWindowFlags_NoResize;
  flags |= ImGuiWindowFlags_NoMove;
  flags |= ImGuiWindowFlags_NoCollapse;
  flags |= ImGuiWindowFlags_AlwaysAutoResize;
  flags |= ImGuiWindowFlags_NoBackground;
  flags |= ImGuiWindowFlags_NoSavedSettings;

  ImGui::SetNextWindowPos(ImVec2{0.0f,0.0f});
  ImGui::SetNextWindowSize(ImVec2{window_w/2.0f, float(window_h)});
  ImGui::Begin("Left Window", nullptr, flags);
  ImGui::TextUnformatted("ctrl-q to quit");
  ImGui::TextUnformatted(app->device_name.c_str());
  ImGui::Value("channels", app->output_channels);
  ImGui::Value("sampling rate", app->sampling_rate);
  ImGui::Value("buffer size", app->buffer_size);
  ImGui::TextUnformatted(app->midiDeviceName.c_str());
  ImGui::Value("mx", cx);
  ImGui::Value("my", cy);
  for (int i = 0; i < NUM_TOGGLES_MAX; i += 1) {
    toggle* t = app->toggles + i;
    if (t->name.empty()) continue;
      ImGui::Checkbox(t->name.c_str(), &t->val);
  }
  for (int i = 0; i < NUM_TRIGGERS_MAX; i += 1) {
    trigger* t = app->triggers + i;
    if (t->name.empty()) continue;
      t->val = ImGui::Button(t->name.c_str()); 
  }
  for(int i = 0; i < NUM_DROPDOWNS_MAX; i += 1){
    dropDown* t = app->dropDowns + i;
    if (t->name.empty()) continue;
      ImGui::Combo(t->name.c_str(), &t->val, t->content, t->length, 4);
  }//Combo(const char* label, int* current_item, const char* const items[], int items_count, int popup_max_height_in_items = -1);
  ImGui::End();

  ImGui::SetNextWindowPos(ImVec2{window_w/2.0f,0.0f});
  ImGui::SetNextWindowSize(ImVec2{window_w/2.0f, float(window_h)});
  ImGui::Begin("Right Window", nullptr, flags);
  for (int i = 0; i < NUM_SLIDERS_MAX; i += 1) {
    slider* s = app->sliders + i;
    if (s->name.empty()) continue;
      ImGui::SliderFloat(s->name.c_str(), &s->val, s->low, s->high);
  }
  ImGui::End();

  ImGui::Render();

  glViewport(0, 0, display_w, display_h);
  glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  for (int i = 0; i < NUM_SLIDERS_MAX; i += 1) {
    slider* s = app->sliders + i;
    s->atomic_val.store(s->val);
  }
  for (int i = 0; i < NUM_TOGGLES_MAX; i += 1) {
    toggle* t = app->toggles + i;
    t->atomic_val.store(t->val);
  }
  for (int i = 0; i < NUM_TRIGGERS_MAX; i += 1) {
    trigger* t = app->triggers + i;
    t->atomic_val.store(t->val);
  }
  for (int i = 0; i < NUM_DROPDOWNS_MAX; i += 1){
    dropDown* t = app->dropDowns + i;
    t->atomic_val.store(t->val);
  }
}
void deleteApp(PedalApp* app){
  try {
    app->rtaudio.stopStream();
  }
  catch (RtAudioError& e) {
    e.printMessage();
  }
  if (app->rtaudio.isStreamOpen()) {
    app->rtaudio.closeStream(); 
  }
  delete app->rtMidiIn;
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  glfwDestroyWindow(app->window);
  glfwTerminate();
  delete app;
}
void startAudioThread(PedalApp* app){
  app->rtaudio.startStream();
}
void appGetCursorPos(PedalApp* app, float* mx, float* my) {
  *mx = app->cursorx.load();
  *my = app->cursory.load();
}
void appAddSlider(PedalApp* app, int sliderIndex, const char* name,
                  float low, float high, float initialValue) {
  slider* s = app->sliders + sliderIndex;
  s->name = name;
  s->low = low;
  s->high = high;
  s->val = initialValue;
  s->atomic_val.store(initialValue);
}
float appGetSlider(PedalApp* app, int idx) {
  return app->sliders[idx].atomic_val.load();
}
void appAddToggle(PedalApp* app, int toggleIndex, const char* name,
                  bool initialValue) {
  toggle* t = app->toggles + toggleIndex;
  t->name = name;
  t->val = initialValue;
  t->atomic_val.store(initialValue);
}
bool appGetToggle(PedalApp* app, int idx) {
  return app->toggles[idx].atomic_val.load();
}
void appAddTrigger(PedalApp* app, int triggerIndex, const char* name) {
  trigger* t = app->triggers + triggerIndex;
  t->name = name;
  t->val = false;
  t->atomic_val.store(false);
}
bool appGetTrigger(PedalApp* app, int idx) {
  return app->triggers[idx].atomic_val.exchange(false);
}
void appAddDropDown(PedalApp* app, int idx, const char* name,char* content[],int length){
  dropDown* t = app->dropDowns + idx;
  t->name = name;
  t->content = content;
  t->val = 0;
  t->length = length;
  t->atomic_val.store(0);
}
int appGetDropDown(PedalApp* app, int idx){
  return app->dropDowns[idx].atomic_val.load();
}


```
If we were sucessful the app will still build and we are ready to add gui to the PedalSynth.cpp.
In PedalSynth.cpp, we can now add and use a slider
```cpp
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
```
Now we have an easy method to add gui elements and query their values. In this example we have created a slider to control frequency

## Adding Pedal
You likely found this repository by looking at Pedal, but if not, Pedal is a pedagogical audio library created by myself and Kee Youn. It is both a functional audio library and a learning resource. Adding pedal, the CMakeLists will look like the following.
Full CMakeLists.txt so far:
```CMake
add_subdirectory(Pedal)

target_include_directories(pedal_app PUBLIC rtaudio imgui/examples Pedal/include/pedal)
target_link_libraries(pedal_app PUBLIC glfw gl3w imgui rtaudio pedal)
```
That should be it! We can replace what we had in the audio callback with a wavetable saw from pedal.
In PedalSynth.cpp
```cpp
#include "WTSaw.hpp"
WTSaw saw;
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
```
This should veryify that pedal is working. 
## Adding RtMidi

Fortunately adding RtMidi is very similar to adding RtAudio; we will need to provide a callback function for the cpp, and tell RtAudio to take care of the rest.
In CMakeLists.txt
```CMake
set(RTMIDI_BUILD_STATIC_LIBS ON CACHE BOOL "Rtmidi Shared Lib" FORCE)
set(RTMIDI_BUILD_TESTING OFF CACHE BOOL "Rtmidi Testing" FORCE)
set(RTMIDI_TARGETNAME_UNINSTALL
    RTMIDI_UNINSTALL CACHE STRING "Rtmidi Uninstall Target" FORCE)
add_subdirectory(rtmidi)

target_include_directories(pedal_app PUBLIC rtaudio imgui/examples Pedal/include/pedal rtmidi)
target_link_libraries(pedal_app PUBLIC glfw gl3w imgui rtaudio rtmidi pedal)
```
In the cpp we will have to add an include, add a few elements to the PedalApp Struct, and initiate RtMidi. Note that we will have to create another function pointer in the hpp to define the createApp() function.
In PedalApp.hpp:
```cpp
#include <vector>
using defaultMidiInputCallback = void (*)(double deltatime, 
                                   std::vector< unsigned char >* message,
                                   PedalApp* app);
PedalApp* createApp(defaultCallback audioCallback, defaultMidiInputCallback midiInputCallback);
```
In PedalApp.cpp:
```cpp
#include "RtMidi.h"
//----------skip
struct PedalApp{
  GLFWwindow* window;//application window
  RtAudio rtaudio;
  std::string device_name;
  unsigned device_id;
  unsigned input_channels;
  unsigned output_channels;
  unsigned sampling_rate;
  unsigned buffer_size;
  defaultCallback callback;
  RtMidiIn* rtMidiIn;
  defaultMidiInputCallback midiInputCallback;
  std::string midiDeviceName;
  unsigned int numPorts;
  slider sliders[NUM_SLIDERS_MAX];
  toggle toggles[NUM_TOGGLES_MAX];
  trigger triggers[NUM_TRIGGERS_MAX];
  dropDown dropDowns[NUM_DROPDOWNS_MAX];
  std::atomic<float> cursorx;
  std::atomic<float> cursory;
};

static void midiCallback( double deltatime, std::vector< unsigned char > *message, void *userData ){
  std::vector<unsigned char> inputMessage;
  for(int i = 0; i < message->size(); i++){
    inputMessage.push_back(message->at(i));
  }
  auto* app = (PedalApp*)userData;
  if(app && app->midiInputCallback){
    app->midiInputCallback(deltatime, message, app);
  }
}
```
and in createApp()
```cpp
//RTMidi
  app->rtMidiIn = new RtMidiIn();
    // Check available ports.
  app->numPorts = app->rtMidiIn->getPortCount();
  for(int i = 0; i < app->numPorts; i++){
    std::cout << "Port " << i << ": " << app->rtMidiIn->getPortName(i) << std::endl;
  }
  if ( app->numPorts == 0 ) {
    std::cout << "No ports available!\n";
    //DELETE STUFF TODO
  }else{
    app->rtMidiIn->openPort(1);
    // Set our callback function.  This should be done immediately after
    // opening the port to avoid having incoming messages written to the
    // queue.
    app->rtMidiIn->setCallback((RtMidiIn::RtMidiCallback)midiCallbackIn);
    app->midiInputCallback = midiCallbackIn;
    app->midiDeviceName = app->rtMidiIn->getPortName(1);
    // Don't ignore sysex, timing, or active sensing messages.
    app->rtMidiIn->ignoreTypes( true, false, false );
  }
```
Now we are ready to add a midiCallback function in the main file
In PedalSynth.cpp
```cpp
void midiCallback(double deltaTime, std::vector<unsigned char>* message, PedalApp* app){
  unsigned int nBytes = message->size();
  for ( unsigned int i=0; i<nBytes; i++ )
    std::cout << "Byte " << i << " = " << (int)message->at(i) << ", ";
  if ( nBytes > 0 )
    std::cout << "stamp = " << deltaTime << std::endl;
}
```
And make sure to add the function pointer to createApp in the main function.
```cpp
PedalApp* app = createApp(audioCallback, midiCallback);
```
Now when the app runs, a midi device should be listed on the window and the input should print to the screen. 

RtMidi provides the binary input data. Each callback is a full MIDI message. You can use pedal's MIDIEvent class to interpret the binary data and get more familiar results

Let's start by using NoteON messages to determine the frequency of the oscillator

In PedalSynth.cpp
```cpp
#include "utilities.hpp"
#include "MIDIEvent.hpp"
MIDIEvent midiEvent;
void midiCallback(double deltaTime, std::vector<unsigned char>* message, PedalApp* app){
  MIDIEvent event(message);
  switch(event.getEventType()){
    case MIDIEvent::EventTypes::NOTE_ON:
    saw.setFrequency(mtof(event.getNoteNumber()));
    std::cout << "Note On | " << event.getNoteNumber() << " " << event.getNoteVelocity() << std::endl;
    break;
  }
}
```
Instead of jumping to the frequency immediately, let's use a smoothvalue. If the time is set to be very short on a smoothvalue, there is no interpreted lag. However, the time can be expanded to intentionally lag and create portamento.
Here is the full PedalSynth.cpp so far:
```cpp
#include "PedalApp.hpp"
#include <iostream>

#define _USE_MATH_DEFINES
#include <cmath>

#include "WTSaw.hpp"
#include "utilities.hpp"
#include "MIDIEvent.hpp"

WTSaw saw;
SmoothValue<float> frequency;
MIDIEvent midiEvent;

void midiCallback(double deltaTime, std::vector<unsigned char>* message, PedalApp* app){
  MIDIEvent event(message);
  switch(event.getEventType()){
    case MIDIEvent::EventTypes::NOTE_ON:
    frequency.setTarget(mtof(event.getNoteNumber()));
    std::cout << "Note On | " << event.getNoteNumber() << " " << event.getNoteVelocity() << std::endl;
    break;
  }
}
void audioCallback(float* output,float* input, unsigned bufferSize, unsigned samplingRate, unsigned outputChannels,
              unsigned inputChannels, double time, PedalApp* app) {
  frequency.setTime(appGetSlider(app, 0));
  for(int i = 0; i < bufferSize; i++){
    saw.setFrequency(frequency.process());
    float currentSample = saw.generateSample() * 0.1f;
    for(int j = 0; j < outputChannels; j++){
      output[i * outputChannels + j] = currentSample;
    }
  }
}
int main(){
  PedalApp* app = createApp(audioCallback, midiCallback);
  appAddSlider(app, 0, "Portamento", 0.0f, 2000.0f, 500.0f);
  startAudioThread(app);
  while(runApp(app)){
    updateApp(app);
  }
  deleteApp(app);
}
```
Lastly, we should use note on and note off messages to manage an envelope. We will use CREnvelope from pedal

Here is the full and final PedalSynth.cpp
```cpp
#include "PedalApp.hpp"
#include <iostream>

#define _USE_MATH_DEFINES
#include <cmath>

#include "WTSaw.hpp"
#include "utilities.hpp"
#include "MIDIEvent.hpp"
#include "CREnvelope.hpp"

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
    std::cout << "Note On | " << event.getNoteNumber() << " " << event.getNoteVelocity() << std::endl;
    break;
    case MIDIEvent::EventTypes::NOTE_OFF:
    std::cout << "Note Off | " << event.getNoteNumber() << " " << event.getNoteVelocity() << std::endl;
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
      output[i * outputChannels + j] = currentSample * 1.0f;
    }
  }
}
int main(){
  PedalApp* app = createApp(audioCallback, midiCallback);
  appAddSlider(app, 0, "Portamento", 0.0f, 2000.0f, 500.0f);
  startAudioThread(app);
  while(runApp(app)){
    updateApp(app);
  }
  deleteApp(app);
}
```