#include "ShaderData.h"
#include "Logger.h"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

ShaderData::ShaderData(const char* vertFile, const char* fragFile){
  // compile shaders
  GLuint vert = compileShader(vertFile, GL_VERTEX_SHADER);
  GLuint frag = compileShader(fragFile, GL_FRAGMENT_SHADER);
  if(vert == 0 || frag == 0){
    return;
  }

  // create program
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
    LOG_ERROR << "Shader linking failed! infoLog: " << infoLog;
    return;
  }

  GLint binaryLength = 0;
  glGetProgramiv(shaderObjectID, GL_PROGRAM_BINARY_LENGTH, &binaryLength);
  bytesSize = (size_t)binaryLength;

  // delete old shaders (they are now linked)
  glDeleteShader(vert);
  glDeleteShader(frag);

  id = shaderObjectID;
}

GLuint ShaderData::compileShader(const char* code, unsigned int type){
  GLuint shaderID;
  int success;
  char infoLog[512];

  shaderID = glCreateShader(type);
  glShaderSource(shaderID, 1, &code, NULL);

  glCompileShader(shaderID);

  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
  if(!success){
    glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
    LOG_ERROR << "Compiling shader ("<< type << ") failed! infoLog: " << infoLog;
    return 0;
  }

  return shaderID;
}

// New method-based API implementation
void ShaderData::use() {
  glUseProgram(id);
}

void ShaderData::reset() {
  glUseProgram(0);
}

void ShaderData::setVec3(const char* name, const glm::vec3& value) {
  glUniform3fv(glGetUniformLocation(id, name), 1, glm::value_ptr(value));
}

void ShaderData::setVec4(const char* name, const glm::vec4& value) {
  glUniform4fv(glGetUniformLocation(id, name), 1, glm::value_ptr(value));
}

void ShaderData::setBool(const char* name, bool value) {
  glUniform1i(glGetUniformLocation(id, name), (int)value);
}

void ShaderData::setInt(const char* name, int value) {
  glUniform1i(glGetUniformLocation(id, name), value);
}

void ShaderData::setFloat(const char* name, float value) {
  glUniform1f(glGetUniformLocation(id, name), value);
}

void ShaderData::setMatrix4(const char* name, const glm::mat4& value) {
  glUniformMatrix4fv(glGetUniformLocation(id, name), 1, GL_FALSE, glm::value_ptr(value));
}


