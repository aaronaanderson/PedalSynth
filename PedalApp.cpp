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