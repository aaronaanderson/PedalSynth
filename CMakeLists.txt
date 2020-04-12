cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(PedalSynth)

find_package(OpenGL REQUIRED)

#GLFW, RTAudio, and IMGUI portions from Kee's work on pedal
# Build all dependencies as static libraries
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries" FORCE)
if(MSVC)
    # suppress general warning on C functions such as strcpy, etc.
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# glfw 
set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW Documentation" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "Installation Target" FORCE)
add_subdirectory(glfw)

# Rtaudio 
set(RTAUDIO_BUILD_STATIC_LIBS ON CACHE BOOL "Rtaudio Shared Lib" FORCE)
set(RTAUDIO_BUILD_TESTING OFF CACHE BOOL "Rtaudio Testing" FORCE)
set(RTAUDIO_TARGETNAME_UNINSTALL
    RTAUDIO_UNINSTALL CACHE STRING "Rtaudio Uninstall Target" FORCE)
add_subdirectory(rtaudio)

# imgui
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

add_library(pedal_app STATIC 
  PedalApp.cpp
  imgui/examples/imgui_impl_glfw.cpp 
  imgui/examples/imgui_impl_opengl3.cpp
)
set_target_properties(pedal_app PROPERTIES
  DEBUG_POSTFIX d
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED ON 
  ARCHIVE_OUTPUT_DIRECTORY lib
  ARCHIVE_OUTPUT_DIRECTORY_DEBUG lib
  ARCHIVE_OUTPUT_DIRECTORY_RELEASE lib 
)
# pedal
add_subdirectory(Pedal)

target_include_directories(pedal_app PUBLIC rtaudio imgui/examples pedal/include)
target_link_libraries(pedal_app PUBLIC glfw gl3w imgui rtaudio pedal)

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