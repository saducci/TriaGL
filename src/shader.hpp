#ifndef SHADER_HPP
#define SHADER_HPP
#include <glm/glm.hpp>
#include <iostream>
namespace shader {
class Shader_class {
private:
  unsigned int m_programID = 0;

public:
  std::string readFile(const char *filepath);

  unsigned int SETUPVERTEXSHADER(const char *source);
  unsigned int SETUPFRAGMNETSHADER(const char *source);
  unsigned int getID();

  void CREATE_SHADER_PROGRAM(const char *vertexpath, const char *fragmentpath);
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
  void use();
  void destroy();
  void setVec2(const std::string &name, const glm::vec2 &value)const ;
  void setVec2(const std::string &name, float x, float y)const ;
  // ------------------------------------------------------------------------
  void setVec3(const std::string &name, const glm::vec3 &value)const ;
  void setVec3(const std::string &name, float x, float y, float z)const ;
  // ------------------------------------------------------------------------
  void setVec4(const std::string &name, const glm::vec4 &value)const;
  void setVec4(const std::string &name, float x, float y, float z, float w)const;
  // ------------------------------------------------------------------------
  void setMat2(const std::string &name, const glm::mat2 &mat)const ;
  // ------------------------------------------------------------------------
  void setMat3(const std::string &name, const glm::mat3 &mat)const;
  // ------------------------------------------------------------------------
  void setMat4(const std::string &name, const glm::mat4 &mat)const;
};
} // namespace shader

#endif
