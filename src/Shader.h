#pragma once
#include <string>
#include <glm/mat4x4.hpp>

class Shader{
public:
  Shader(const char* vertexPath, const char* fragmentPath); 
  ~Shader();

public:
  void Use();
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;

public:
  void SetMatrix4(glm::mat4 aMatrix, const std::string& aName);
public:
  // the program ID
  unsigned int ID;
};
