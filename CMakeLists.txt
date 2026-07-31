
CMAKE_MINIMUM_REQUIRED(VERSION 3.0.0)
PROJECT(learnopenglC++)
ADD_EXECUTABLE(learnopenglC++
  src/main.cpp
  src/glad.c
  src/config.h

)
TARGET_INCLUDE_DIRECTORIES(learnopenglC++ PRIVATE include)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

target_link_libraries(learnopenglC++
    glfw
    OpenGL::GL
)
