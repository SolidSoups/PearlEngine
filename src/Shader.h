#pragma once
#include <string>

class Shader{
public:
  Shader(const char* aVertexPath, const char* aFragmentPath); 
  ~Shader();
private:
  std::string LoadShader(const char* aPath);
  unsigned int LoadVertexShader(const char* aPath);
  unsigned int LoadFragmentShader(const char* aPath);
public:
  unsigned int shaderProgram;
  void Use();
};
