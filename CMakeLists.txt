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

set_target_properties(pedal_app PROPERTIES
  DEBUG_POSTFIX d
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON 
  ARCHIVE_OUTPUT_DIRECTORY lib
  ARCHIVE_OUTPUT_DIRECTORY_DEBUG lib
  ARCHIVE_OUTPUT_DIRECTORY_RELEASE lib 
)

target_link_libraries(pedal_app PUBLIC glfw)
add_executable(PedalSynth PedalSynth.cpp)
set_target_properties(PedalSynth PROPERTIES
  DEBUG_POSTFIX d
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON
  RUNTIME_OUTPUT_DIRECTORY bin
  RUNTIME_OUTPUT_DIRECTORY_DEBUG bin
  RUNTIME_OUTPUT_DIRECTORY_RELEASE bin
)
target_link_libraries(PedalSynth PRIVATE pedal_app)