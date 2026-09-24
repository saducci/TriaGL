#include "config.h"
#include <GLFW/glfw3.h>
#include <cstddef>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/trigonometric.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "shader.hpp"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Vector3 {
  float x, y, z;
};
struct Vector2 {
  float x, y;
};
constexpr unsigned int SCREEN_WIDTH = 640;
constexpr unsigned int SCREEN_HEIGHT = 480;

// functions
void FRAMEBUFFER_SIZE_CALLBACK(GLFWwindow *window, int width, int height);
void PROCESS_INPUT(GLFWwindow *window, float &mixValue);
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
  static constexpr unsigned int indices[] = {

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
  // void draw() {
  //   glBindVertexArray(m_VAO);
  // }
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
  bool LOAD_TEXTURE_IMAGE(const char *path_to_image) {
    stbi_set_flip_vertically_on_load(true);
    stbi_data = stbi_load(path_to_image, &texture_width, &texture_height,
                          &texture_nrChannels, 0);
    if (!stbi_data) {
      std::cout << "Failed to load texture: " << path_to_image << "\n";
      return false;
    }
    return true;
  }
  void CREATE_TEXTURE(GLint param) {
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
  void BIND(unsigned int textureUnit) const {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, texture);
  };
  void delete_texture() { glDeleteTextures(1, &texture); }
  ~texture_class() { delete_texture(); }
};
;
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

  // see if the window was resized call the framebuffer_size_callback function
  // and reset the glviewport
  // glfwSetFramebufferSizeCallback(window, FRAMEBUFFER_SIZE_CALLBACK);
  // create the render loop
  // vertices for drawing a triangle

  shader::Shader_class default_shader_1;
  default_shader_1.CREATE_SHADER_PROGRAM("./src/vertexshader.vert",
                                         "./src/fragment.glsl");
  default_shader_1.use();

  default_shader_1.setInt("texture0", 0);
  default_shader_1.setInt("texture1", 1);

  Mesh_class triangle;
  triangle.create(6);
  texture_class wood_texture;
  wood_texture.LOAD_TEXTURE_IMAGE("./src/wall.jpg");
  wood_texture.CREATE_TEXTURE(GL_REPEAT);

  texture_class awesomeface_texture;
  awesomeface_texture.LOAD_TEXTURE_IMAGE("./src/awesomeface.png");
  awesomeface_texture.CREATE_TEXTURE(GL_REPEAT);

  // uniform
  // GLM

  float mixValue = 0.2f;

  while (!glfwWindowShouldClose(window)) {

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    // input
    //
    glfwSetFramebufferSizeCallback(window, FRAMEBUFFER_SIZE_CALLBACK);
    PROCESS_INPUT(window, mixValue);
    // rendering cmd here

    float localxoffset = -0.0;
    float localyoffset = 0;
    // shader things

    default_shader_1.setFloat("mixValue", mixValue);
    // drawing things

    wood_texture.BIND(0);
    awesomeface_texture.BIND(1);

    // glm::vec3 pos = glm::vec3(0.0, 0.0, 0.0);
    glm::mat4 trans = glm::mat4(1.0f);
    trans =
        glm::rotate(trans, (float)glfwGetTime() * 2, glm::vec3(0.0, 0.0, 1.0f));
    // trans = glm::scale(trans, glm::vec3(1.0, 1.0, 1.0));
    // trans = glm::translate(trans, pos);
    

    default_shader_1.setMat4("transform", trans);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // glBindVertexArray(triangle.getVAO());
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // GLFW
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  default_shader_1.destroy();
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
void FRAMEBUFFER_SIZE_CALLBACK(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}
void PROCESS_INPUT(GLFWwindow *window, float &mixValue) {
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
