// src
#include <Shader.h>

// lib
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

// std
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>

Shader::Shader(const char* vertexPath, const char* fragmentPath){
  // load shaders from file
  std::string vertexCode, fragmentCode;
  std::ifstream vShaderFile, fShaderFile;
  // ensure ifstreams can throw exceptions;
  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try{
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);
    std::stringstream vShaderStream, fShaderStream;

    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    vShaderFile.close();
    fShaderFile.close();

    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
  }
  catch(std::ifstream::failure e){
    std::cerr << "Shader::Shader(..) -> File not succesfully read\n" << e.what() << std::endl;
  }

  const char* vShaderCode = vertexCode.c_str();
  const char* fShaderCode = fragmentCode.c_str();

  // compile shaders
  unsigned int vertex, fragment;
  int success;
  char infoLog[512];

  // vertex shader
  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);

  // print vertex compilation errors
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if(!success){
    glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    std::cerr << "Shader::Shader() -> Vertex compilation failed!\n" << infoLog << std::endl;
  };

  // fragment shader
  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(fragment);

  // print fragment compilation errors
  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
  if(!success){
    glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    std::cerr << "Shader::Shader() -> Fragment compilation failed!\n" << infoLog << std::endl;
  };

  // shader program
  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);

  // print linking errors if any
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if(!success){
    glGetProgramInfoLog(ID, 512, NULL, infoLog);
    std::cout << "Shader::Shader() -> Program linking failed!\n" << infoLog << std::endl;
  }

  // delete the shaders as they're linked and no longer needed
  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

Shader::~Shader(){
  glDeleteProgram(ID);
  std::cout << "Shader::~Shader() -> Deleted shader program" << std::endl;
}

void Shader::Use(){
  glUseProgram(ID);
}


void Shader::SetVec3(const std::string& name, const glm::vec3& value) const{
  glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::SetVec4(const std::string& name, const glm::vec4& value) const{
  glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, glm::value_ptr(value));
}

void Shader::SetBool(const std::string &name, bool value) const{
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string &name, int value) const{
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string &name, float value) const{
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}



void Shader::SetMatrix4(glm::mat4 aMatrix, const std::string& aName){
  glUniformMatrix4fv(glGetUniformLocation(ID, aName.c_str()), 1, GL_FALSE, glm::value_ptr(aMatrix));
}
