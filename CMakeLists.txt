cmake_minimum_required (VERSION 3.0)
project (AudioVisualCpp)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)

include_directories(src)
include_directories(external)
include_directories(${OPENGL_INCLUDE_DIR})

set(SOURCES 
   src/main.cpp
   src/render_object.h
   src/cube.h
   src/common.h
)
add_executable(AudioVisualCpp ${SOURCES})

target_link_libraries(AudioVisualCpp glfw)
target_link_libraries(AudioVisualCpp ${OPENGL_gl_LIBRARY})
