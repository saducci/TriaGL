#include "shader.hpp"
#include "config.h"
#include <glm/gtc/type_ptr.hpp>
namespace shader {

std::string Shader_class::readFile(const char *filepath) {
  std::ifstream Shaderfile(filepath);

  if (!Shaderfile.is_open()) {
    std::cout << "Failed to open" + std::string(filepath) + "\n";
    return "";
  }
  std::stringstream buffer;
  buffer << Shaderfile.rdbuf();
  return buffer.str();
}
unsigned int Shader_class::SETUPVERTEXSHADER(const char *source) {
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
unsigned int Shader_class::SETUPFRAGMNETSHADER(const char *source) {
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
void Shader_class::CREATE_SHADER_PROGRAM(const char *vertexpath,
                                         const char *fragmentpath) {
  std::string vertexCode = readFile(vertexpath);
  std::string fragmentCode = readFile(fragmentpath);
  unsigned int vertexShader = SETUPVERTEXSHADER(vertexCode.c_str());
  unsigned int fragmentShader = SETUPFRAGMNETSHADER(fragmentCode.c_str());
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
unsigned int Shader_class::getID() { return m_programID; }
void Shader_class::setBool(const std::string &name, bool value) const {
  glUniform1i(glGetUniformLocation(m_programID, name.c_str()), (int)value);
};
void Shader_class::setInt(const std::string &name, int value) const {
  glUniform1i(glGetUniformLocation(m_programID, name.c_str()), value);
};
void Shader_class::setFloat(const std::string &name, float value) const {
  glUniform1f(glGetUniformLocation(m_programID, name.c_str()), value);
};
void Shader_class::setVec2(const std::string &name, const glm::vec2 &value) const {
  glUniform2fv(glGetUniformLocation(m_programID, name.c_str()), 1, &value[0]);
};
void Shader_class::setVec2(const std::string &name, float x, float y) const {
  glUniform2f(glGetUniformLocation(m_programID, name.c_str()), x, y);
};
// ------------------------------------------------------------------------
void Shader_class::setVec3(const std::string &name, const glm::vec3 &value) const {
  glUniform3fv(glGetUniformLocation(m_programID, name.c_str()), 1, &value[0]);
};
void Shader_class::setVec3(const std::string &name, float x, float y, float z) const {
  glUniform3f(glGetUniformLocation(m_programID, name.c_str()), x, y, z);
};
// ------------------------------------------------------------------------
void Shader_class::setVec4(const std::string &name, const glm::vec4 &value) const {
  glUniform4fv(glGetUniformLocation(m_programID, name.c_str()), 1, &value[0]);
};
void Shader_class::setVec4(const std::string &name, float x, float y, float z,
             float w) const {
  glUniform4f(glGetUniformLocation(m_programID, name.c_str()), x, y, z, w);
};
// ------------------------------------------------------------------------
void Shader_class::setMat2(const std::string &name, const glm::mat2 &mat) const {
  glUniformMatrix2fv(glGetUniformLocation(m_programID, name.c_str()), 1, GL_FALSE,
                     &mat[0][0]);
};
// ------------------------------------------------------------------------
void Shader_class::setMat3(const std::string &name, const glm::mat3 &mat) const {
  glUniformMatrix3fv(glGetUniformLocation(m_programID, name.c_str()), 1, GL_FALSE,
                     &mat[0][0]);
};
// ------------------------------------------------------------------------
void Shader_class::setMat4(const std::string &name, const glm::mat4 &mat) const {
  glUniformMatrix4fv(glGetUniformLocation(m_programID, name.c_str()), 1, GL_FALSE,
                     &mat[0][0]);
};

void Shader_class::use() { glUseProgram(m_programID); }
void Shader_class::destroy() { glDeleteProgram(m_programID); }
}; // namespace shader
