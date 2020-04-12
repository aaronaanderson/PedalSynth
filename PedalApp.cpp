#include "PedalApp.hpp"

#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

#include "RtAudio.h"
#include <string>
#include <atomic>
#include <iostream>

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
  std::atomic<float> cursorx;
  std::atomic<float> cursory;
};

//==============This is needed for glfw window callback
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
  if ((action == GLFW_PRESS) && (mods & GLFW_MOD_CONTROL) && (key == GLFW_KEY_Q)) {
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
PedalApp* createApp(defaultCallback callback){
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