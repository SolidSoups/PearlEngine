// src
#include <Shader.h>

// lib
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// std
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader(const char* aVertexPath, const char* aFragmentPath){
  int result;
  char Log[512];

  unsigned int vertexShader = LoadVertexShader(aVertexPath);
  unsigned int fragmentShader = LoadFragmentShader(aFragmentPath);

  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &result);
  if(!result){
    glGetProgramInfoLog(shaderProgram, 512, NULL, Log);
    std::cerr << "Shader::Shader(..) -> Shader program could not compile\n" << Log << std::endl;
  }
  else{
    std::cout << "Shader::Shader(..) -> Succesfully compiled shader program" << std::endl;
  }

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
}

Shader::~Shader(){
  glDeleteProgram(shaderProgram);
  std::cout << "Shader::~Shader() -> Deleted shader program" << std::endl;
}

std::string Shader::LoadShader(const char* aPath){
  std::ifstream shaderFile(aPath);
  if(!shaderFile.is_open()){
    std::cerr << "Shader::LoadShader(..) -> Failed to open file at path '" << aPath << "'" << std::endl;
    return "";
  }

  std::stringstream shaderStream;
  shaderStream << shaderFile.rdbuf();
  return shaderStream.str();
}

unsigned int Shader::LoadVertexShader(const char* aPath){
  int result;
  char Log[512];

  std::string shaderCodeString = LoadShader(aPath);
  const char* shaderCode = shaderCodeString.c_str();

  unsigned int shaderObject;

  shaderObject = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(shaderObject, 1, &shaderCode, NULL);
  glCompileShader(shaderObject);

  glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);

  if(!result){
    glGetShaderInfoLog(shaderObject, 512, NULL, Log);
    std::cout << "Shader::LoadVertexShader(..) -> Failed to compile vertex shader \n" << Log << std::endl;
  }

  return shaderObject;
}




unsigned int Shader::LoadFragmentShader(const char* aPath){
  int result;
  char Log[512];

  std::string shaderCodeString = LoadShader(aPath);
  const char* shaderCode = shaderCodeString.c_str();

  unsigned int shaderObject;

  shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(shaderObject, 1, &shaderCode, NULL);
  glCompileShader(shaderObject);

  glGetShaderiv(shaderObject, GL_COMPILE_STATUS, &result);

  if(!result){
    glGetShaderInfoLog(shaderObject, 512, NULL, Log);
    std::cout << "Shader::LoadFragmentShader(..) -> Failed to compile fragment shader \n" << Log << std::endl;
  }

  return shaderObject;
}

void Shader::Use(){
  glUseProgram(shaderProgram);
}

