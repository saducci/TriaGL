#include"config.h"
#include <GLFW/glfw3.h>
#include <cstddef>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
using std::string;
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
void processInput(GLFWwindow *window, float &mixValue);

//

class Mesh_class {
private:
  unsigned int m_VAO = 0;
  unsigned int m_VBO = 0;

  unsigned int m_EBO = 0;
  unsigned int m_indexcount = 0;

public:
  // you can change the primitve here  so it can draw other things

  static constexpr float vertices[] = {
      // positions          // colors           // texture coords
      0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 2.0f, 2.0f, // top right
      0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 2.0f, 0.0f, // bottom right
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
      -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 2.0f  // top left
  };
  static constexpr unsigned int  indices[] = {

      3, 1, 0, 3, 2, 1};
  GLenum primitive = GL_TRIANGLES;
  // void create(const float *vertices, size_t vertexsize,
  //             const unsigned int *indices, size_t indexsize,
  //             unsigned int count) {
  void create(unsigned int count) {
    m_indexcount = count;
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                          (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
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
  unsigned int getVAO() const { return m_VAO; }
  unsigned int getVBO() const { return m_VBO; }
  ~Mesh_class() { destroy(); }
};
class texture_class {
private:
  unsigned int texture;
  unsigned char *stbi_data = nullptr;
  int texture_width = 0, texture_height = 0, texture_nrChannels = 0;

public:
  bool load_texture_image(const char *path_to_image) {
    stbi_set_flip_vertically_on_load(true);
    stbi_data = stbi_load(path_to_image, &texture_width, &texture_height,
                          &texture_nrChannels, 0);
    if (!stbi_data) {
      std::cout << "Failed to load texture: " << path_to_image << "\n";
      return false;
    }
    return true;
  }
  void create_texture(GLint param) {
    float borderColor[] = {0.2f, 0.3f, 0.3f, 1.0f};
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, param);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, param);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (stbi_data) {
      GLenum format = (texture_nrChannels == 4) ? GL_RGBA : GL_RGB;
      glTexImage2D(GL_TEXTURE_2D, 0, format, texture_width, texture_height, 0,
                   format, GL_UNSIGNED_BYTE, stbi_data);
      glGenerateMipmap(GL_TEXTURE_2D);
      stbi_image_free(stbi_data);
    } else {
      std::cout << "Failed to load texture";
    }
  }
  void bind(unsigned int textureUnit) const {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, texture);
  };
  void delete_texture() { glDeleteTextures(1, &texture); }
  ~texture_class() { delete_texture(); }
};
;
class Shader_class {
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
      glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "TriaGL", NULL, NULL);

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

  Shader_class shader;
  shader.createShaderProgram("./src/vertexshader.vert", "./src/fragment.glsl");
  shader.use();

  shader.setInt("texture0", 0);
  shader.setInt("texture1", 1);

  Mesh_class triangle;
  triangle.create(6);
  texture_class wood_texture;
  wood_texture.load_texture_image("./src/wall.jpg");
  wood_texture.create_texture(GL_REPEAT);

  texture_class awesomeface_texture;
  awesomeface_texture.load_texture_image("./src/awesomeface.png");
  awesomeface_texture.create_texture(GL_REPEAT);

  // uniform

  float mixValue = 0.2f;

  while (!glfwWindowShouldClose(window)) {

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // input
    processInput(window, mixValue);
    // rendering cmd here

    float localxoffset = -0.0;
    float localyoffset = 0;
    // shader things

    shader.setFloat("mixValue", mixValue);
    shader.setFloat("u_localyoffset", localyoffset);
    shader.setFloat("u_localxoffset", localxoffset);
    // drawing things

    wood_texture.bind(0);
    awesomeface_texture.bind(1);
    triangle.draw();

    // glBindVertexArray(triangle.getVAO());
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // GLFW
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
void processInput(GLFWwindow *window, float &mixValue) {
  const float randomnumber = 0.05f;

  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
  if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
    mixValue += randomnumber;
    if (mixValue >= 1.0f) {
      mixValue = 1.0f;
    }
  }
  if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
    mixValue -= randomnumber;
    if (mixValue <= 0.0f)
      mixValue = 0.0f;
  }
}
