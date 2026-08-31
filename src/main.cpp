#include "config.h"
using std::string;
typedef unsigned NON_NEGATIVE;
struct Vector3 {
  float x, y, z;
};
struct Vector2 {
  float x, y;
};
constexpr unsigned int SCREEN_WIDTH = 640;
constexpr unsigned int SCREEN_HEIGHT = 480;

// functions
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

//

class Mesh {
private:
  unsigned int m_VAO = 0;
  unsigned int m_VBO = 0;

  unsigned int m_EBO = 0;
  unsigned int m_indexcount = 0;

public:
  // you can change the primitve here  so it can draw other things
  GLenum primitive = GL_TRIANGLES;
  void create(const float *vertices, size_t vertexsize,
              const unsigned int *indices, size_t indexsize,
              unsigned int count) {
    m_indexcount = count;
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);

    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexsize, vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexsize, indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
  }
  void draw() {
    glBindVertexArray(m_VAO);
    glDrawElements(primitive, m_indexcount, GL_UNSIGNED_INT, 0);
  }
  void destroy() {
    glDeleteVertexArrays(1, &m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_EBO);
  }
  ~Mesh() { destroy(); }
};
class Shader {
private:
  unsigned int m_programID = 0;

public:
  string readFile(const char *filepath) {
    std::ifstream Shaderfile(filepath);

    if (!Shaderfile.is_open()) {
      std::cout << "Failed to open" + string(filepath) + "\n";
      return "";
    }
    std::stringstream buffer;
    buffer << Shaderfile.rdbuf();
    return buffer.str();
  }
  unsigned int setupVertexShader(const char *source) {
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &source, NULL);
    glCompileShader(vertexShader);
    int success;
    char infolog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(vertexShader, sizeof(infolog), NULL, infolog);
      std::cout << "ERROR::SHADER:VERTEX::COMPIPLATION_FAILED\n"
                << infolog << "\n";
    }

    return vertexShader;
  }
  unsigned int setupFragmentShader(const char *source) {
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &source, NULL);
    glCompileShader(fragmentShader);
    int success;
    char infolog[1024];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
      glGetShaderInfoLog(fragmentShader, sizeof(infolog), NULL, infolog);
      std::cout << "ERROR::SHADER:FRAGMENT::COMPILATION_FAILED\n"
                << infolog << "\n";
    }
    return fragmentShader;
  }
  void createShaderProgram(const char *vertexpath, const char *fragmentpath) {
    string vertexCode = readFile(vertexpath);
    string fragmentCode = readFile(fragmentpath);
    unsigned int vertexShader = setupVertexShader(vertexCode.c_str());
    unsigned int fragmentShader = setupFragmentShader(fragmentCode.c_str());
    m_programID = glCreateProgram();
    glAttachShader(m_programID, vertexShader);
    glAttachShader(m_programID, fragmentShader);
    glLinkProgram(m_programID);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    int success;
    char infolog[512];
    glGetProgramiv(m_programID, GL_LINK_STATUS, &success);
    if (!success) {
      glGetProgramInfoLog(m_programID, 512, NULL, infolog);
      std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infolog << '\n';
    }
  }
  unsigned int getID() { return m_programID; }
  void setBool(const string &name, bool value) const {
    glUniform1i(glGetUniformLocation(m_programID, name.c_str()), (int)value);
  };
  void setInt(const string &name, int value) const {
    glUniform1i(glGetUniformLocation(m_programID, name.c_str()), value);
  };
  void setFloat(const string &name, float value) const {
    glUniform1f(glGetUniformLocation(m_programID, name.c_str()), value);
  };

  void use() { glUseProgram(m_programID); }
  void destroy() { glDeleteProgram(m_programID); }
};

int main() {
  // init the glfw
  glfwInit();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  // creating a pointer to GLFWwindow

  // object and creating glfw window
  GLFWwindow *window =
      glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "HELLO WORLD ", NULL, NULL);

  if (window == NULL) {
    std::cout << "Failed to create GLFW window\n";
    glfwTerminate();
    return -1;
  }
  // making the curent context with the window(the pointer to GLFWwindow object

  glfwMakeContextCurrent(
      window); // Loading the GLAD so we can call the OpenGL functions
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD\n";
    return -1;
  }
  // setting the ViewPort
  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

  // see if the window was resized call the framebuffer_size_callback function
  // and reset the glviewport
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  // create the render loop
  // vertices for drawing a triangle
  // float triangle_vertice_1[] = {0.05f, 0.05f,  0.00f, 0.05f, -0.05f,
  //                               0.00f, -0.05f, 0.05f, 0.00f};
  // float triangle_vertice_2[] = {0.02f, -0.05f, 0.00f, 0.03f, -0.03f,
  //                               0.00f, -0.05f, -0.02f, 0.00f};
  float vertices[] = {
      // positions         // colors
      0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
      0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f  // top
  };
  float texture_cords[] = {0.0f, 0.0f,
                           1.0f, 0.0f,
                           0.5,  1.0f};

  unsigned int indices[] = {0, 1, 2};
  // for (int i = 0; i < 3; i++) {
  //   vertices[i * 6 + 0] *= -1; // x
  //   vertices[i * 6 + 1] *= -1; // y
  //                              // vertices[i * 6 + 2] *= -1; // z
  // } // creating a program object
  Shader shader;
  shader.createShaderProgram("./src/vertexshader.vert", "./src/fragment.glsl");
  Mesh triangle1;

  // triangle1.create(triangle_vertice_1, sizeof(triangle_vertice_1), indices,
  //                  sizeof(indices), 3);
  Mesh triangle;
  triangle.create(vertices, sizeof(vertices), indices, sizeof(indices), 3);

  // std::cout << std::filesystem::current_path() << '\n';

  while (!glfwWindowShouldClose(window)) {

    // glClearColor(0.7f, 0.3f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // input
    processInput(window);
    // rendering cmd here

    shader.use();

    float offset = 0.0f;
    shader.setFloat("xoffset", offset);
    float localoffset = 1;
    shader.setFloat("localoffset", localoffset);
    triangle.draw();
    // check&call events & swap the buffers

    glfwSwapBuffers(window);

    glfwPollEvents();
  }
  shader.destroy();
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}
void processInput(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}
