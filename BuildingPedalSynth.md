
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
In PEdalApp.cpp:
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
It is important to be careful with git add .
