#include "ShaderData.h"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

namespace { // empty namespace will only be visible to this translation unit

// Read a shader file
std::string ReadShaderFile(const char* filepath){
  std::string code;
  std::ifstream shaderFile;
  // ensure ifstreams can throw exceptions
  shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try{
    shaderFile.open(filepath);
    std::stringstream ss;

    ss << shaderFile.rdbuf(); 

    shaderFile.close();

    code = ss.str(); 
  }
  catch(std::ifstream::failure e){
    std::cerr << "ShaderData.cpp: ReadShaderFile: File failed to read\n" << e.what() << std::endl; 
    return "";
  }

  return code;
};

enum ShaderType { VERTEX, FRAGMENT };
GLuint CompileShader(const char* code, ShaderType type){
  GLuint shaderID;
  int success;
  char infoLog[512];

  // create shader
  int shaderType = type == VERTEX ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER;
  shaderID = glCreateShader(shaderType);
  glShaderSource(shaderID, 1, &code, NULL);

  // compile shader
  glCompileShader(shaderID);

  // check for errors
  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
  if(!success){
    glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
    std::cerr << "ShaderData.cpp: CompileShader: " << (type == VERTEX ? "Vertex" : "Fragment") << " compilation failed\n" << infoLog << std::endl;
    return 0;
  }

  return shaderID;
}
};

ShaderData CreateShader(const char* vertexPath, const char* fragmentPath){
  std::string vertexCode = ReadShaderFile(vertexPath);
  std::string fragmentCode = ReadShaderFile(fragmentPath);

  const char* vShaderCode = vertexCode.c_str();
  const char* fShaderCode = fragmentCode.c_str();

  // compile shaders
  GLuint vert = CompileShader(vShaderCode, VERTEX);
  GLuint frag = CompileShader(fShaderCode, FRAGMENT);

  // create shader object
  ShaderData shaderData;

  // create shader program
  GLuint shaderObjectID = glCreateProgram();

  // attach and link program with compiled shaders
  glAttachShader(shaderObjectID, vert);
  glAttachShader(shaderObjectID, frag);
  glLinkProgram(shaderObjectID);

  // check for linking errors
  int success;
  char infoLog[512];
  glGetProgramiv(shaderObjectID, GL_LINK_STATUS, &success);
  if(!success){
    glGetProgramInfoLog(shaderObjectID, 512, NULL, infoLog);
    std::cout << "ShaderData.cpp: CreateShader: Program linking failed\n" << infoLog << std::endl;
    return {0};
  }

  // delete the shaders as they're now linked and no longer needed
  glDeleteShader(vert);
  glDeleteShader(frag);

  return {shaderObjectID};
}

void UseShader(ShaderData shaderData){
  glUseProgram(shaderData.id);
}

void ResetShader(){
  glUseProgram(0);
}

void DestroyShader(ShaderData shaderData){
  glDeleteProgram(shaderData.id);
}

// uniforms

void ShaderSetVec3(ShaderData shaderData, const char* name, const glm::vec3& value){
  glUniform3fv(glGetUniformLocation(shaderData.id, name), 1, glm::value_ptr(value));
}
void ShaderSetVec4(ShaderData shaderData, const char* name, const glm::vec4& value){
  glUniform4fv(glGetUniformLocation(shaderData.id, name), 1, glm::value_ptr(value));
}
void ShaderSetBool(ShaderData shaderData, const char* name, bool value){
  glUniform1i(glGetUniformLocation(shaderData.id, name), (int)value);
}
void ShaderSetInt(ShaderData shaderData, const char* name, int value){
  glUniform1i(glGetUniformLocation(shaderData.id, name), value);
}
void ShaderSetFloat(ShaderData shaderData, const char* name, float value){
  glUniform1f(glGetUniformLocation(shaderData.id, name), value);
}
void ShaderSetMatrix4(ShaderData shaderData, const char* name, const glm::mat4& value){
  glUniformMatrix4fv(glGetUniformLocation(shaderData.id, name), 1, GL_FALSE, glm::value_ptr(value));
}


