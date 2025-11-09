#pragma once
#include <string>
#include <glm/mat4x4.hpp>

class Shader{
public:
  Shader(const char* vertexPath, const char* fragmentPath); 
  ~Shader();

public:
  void Use();
  void SetVec3(const std::string& name, const glm::vec3& value) const;
  void SetVec4(const std::string& name, const glm::vec4& value) const;
  void SetBool(const std::string &name, bool value) const;
  void SetInt(const std::string &name, int value) const;
  void SetFloat(const std::string &name, float value) const;

public:
  void SetMatrix4(glm::mat4 aMatrix, const std::string& aName);
public:
  // the program ID
  unsigned int ID;
};
