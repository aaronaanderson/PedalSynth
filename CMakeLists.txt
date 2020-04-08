cmake_minimum_required(VERSION 3.8 FATAL_ERROR)

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