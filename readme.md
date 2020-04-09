# Pedal Synth
## A synth made with pedal
This repo is in preparation for a "How to build an audio app with C++ and cmake" talk. THe main value is the MakingPedalSynth.md document, where I go over each step in creating the build.

### build instructions 
Clone the repo (doesn't matter where)
```
git clone https://github.com/aaronaanderson/PedalSynth.git
```
cd into PedalSynth
```
cd PedalSynth
```
Make a build folder
```
mkdir build
```
cd in to that
```
cd build
```
Now call cmake from inside the build folder. We will provide cmake with the location of the folder with the CMakeLists.txt file
```
cmake ../../PedalSynth
```
This should have generated the generator files. Calling make should build the app
```
make
```
If successful, the app may be executed by calling
```
./bin/PedalSynth
```