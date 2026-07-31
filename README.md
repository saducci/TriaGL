# LearnOpenGL C++

![OpenGL](https://img.shields.io/badge/OpenGL-Graphics-blue)
![C++](https://img.shields.io/badge/C%2B%2B-Programming-orange)
![CMake](https://img.shields.io/badge/CMake-Build_System-green)

# About This Project

Hello, I'm **Saducci**.

This repository is my personal journey of learning **OpenGL and computer graphics using C++**.

The purpose of this project is to understand how graphics programming works at a lower level by building everything step by step without using a game engine.

Through this project, I am learning:

- How the GPU and CPU communicate
- How the OpenGL rendering pipeline works
- How shaders control graphics
- How vertices and buffers are managed
- The foundations needed for game engine development

---

# Technologies Used

- **C++**
- **OpenGL**
- **GLFW**
- **GLAD**
- **GLSL Shaders**
- **CMake**
- **GLM**

---

# Project Structure

```
learnopenglC++
│
├── src/
│   ├── main.cpp              # Main OpenGL application
│   ├── glad.c                # OpenGL loader source
│   ├── config.h              # Common project includes
│   ├── vertexshader.vert     # Vertex shader
│   └── fragment.glsl         # Fragment shader
│
├── include/
│   ├── glad/
│   │   └── glad.h
│   └── KHR/
│       └── khrplatform.h
│
├── CMakeLists.txt
└── README.md
```

---

# Requirements

Before building this project, install:

- A C++ compiler
- CMake
- OpenGL development libraries
- GLFW

For Ubuntu:

```bash
sudo apt install build-essential cmake libglfw3-dev
```

---

# Building

Clone the repository:

```bash
git clone https://github.com/saducci/learnopenglC++.git
cd learnopenglC++
```

Create a build directory:

```bash
mkdir build
cd build
```

Generate build files:

```bash
cmake ..
```

Compile:

```bash
make
```

Run:

```bash
./learnopenglC++
```

---

# Current Features

Currently this project includes:

- OpenGL window creation
- GLFW setup
- GLAD initialization
- Vertex Buffer Objects (VBO)
- Vertex Array Objects (VAO)
- Basic shaders
- Rendering primitives
- Basic OpenGL pipeline understanding

---

# Learning Goals

My goal with this project is to build a strong foundation in:

- Computer graphics
- Game development
- Real-time rendering
- Engine programming
- Low-level C++ programming

---

# Future Plans

Future improvements:

- Textures
- Camera systems
- 3D rendering
- Lighting systems
- Model loading
- Physics experiments
- More advanced OpenGL techniques

---

# About Me

**Saducci**

I am a programmer interested in:

- C++
- Game development
- Computer graphics
- Linux
- Low-level programming

This repository documents my progress while learning graphics programming and building my understanding of how games and rendering systems work.

---

# License

This project is for learning and educational purposes.
```
