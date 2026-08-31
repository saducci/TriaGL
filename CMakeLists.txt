
CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)
PROJECT(TriaGL)
ADD_EXECUTABLE(TriaGL
  src/main.cpp
  src/glad.c
  src/config.h

)
TARGET_INCLUDE_DIRECTORIES(TriaGL PRIVATE include)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

target_link_libraries(TriaGL
    glfw
    OpenGL::GL
)
