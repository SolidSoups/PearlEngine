#include "ShaderData.h"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

#include "Logger.h"
#include "FileSystem.h"

ShaderData::ShaderData(const std::string& vertPath, const std::string &fragPath)
  : m_VertPath(vertPath), m_FragPath(fragPath)
{
  reload();
}

bool ShaderData::reload(){
  if(m_VertPath.empty() || m_FragPath.empty()){
    LOG_WARNING << "Cannot reload shader: no source paths stored";
    return false;
  }

  std::vector<char> vertBytes, fragBytes;
  if(!FileSystem::loadFile(m_VertPath.c_str(), vertBytes)){
    LOG_ERROR << "Failed to load vertex file at path: " << m_FragPath;
    return false;
  }
  if(!FileSystem::loadFile(m_FragPath.c_str(), fragBytes)){
    LOG_ERROR << "Failed to load frag file at path: " << m_VertPath;
    return false;
  }

  // compile shaders
  GLuint newVert = compileShader(vertBytes.data(), GL_VERTEX_SHADER);
  GLuint newFrag = compileShader(fragBytes.data(), GL_FRAGMENT_SHADER);
  if(newVert == 0){
    LOG_ERROR << "Shader compilation failed for vert file '" << m_VertPath << "', keeping old shader (if any)";
    if(newVert) glDeleteShader(newVert);
    if(newFrag) glDeleteShader(newFrag);
    return false;
  }
  if(newFrag == 0){
    LOG_ERROR << "Shader compilation failed for frag file '" << m_FragPath << "', keeping old shader (if any)";
    if(newVert) glDeleteShader(newVert);
    if(newFrag) glDeleteShader(newFrag);
    return false;
  }

  // link new program
  GLuint newProgram = glCreateProgram();
  glAttachShader(newProgram, newVert);
  glAttachShader(newProgram, newFrag);
  glLinkProgram(newProgram);

  int success;
  char infoLog[512];
  glGetProgramiv(newProgram, GL_LINK_STATUS, &success);
  if(!success){
    glGetProgramInfoLog(newProgram, 512, NULL, infoLog);
    LOG_ERROR << "Shader linking failed! infoLog: " << infoLog;
    glDeleteProgram(newProgram);
    glDeleteShader(newVert);
    glDeleteShader(newFrag);
    return false;
  }

  if(id) glDeleteProgram(id);
  id = newProgram;

  // delete old shaders (they are now linked)
  glDeleteShader(newVert);
  glDeleteShader(newFrag);

  id = newProgram;
  return true;
}

GLuint ShaderData::compileShader(const char* code, unsigned int type){
  GLuint shaderID;
  int success;
  char infoLog[512];
  std::string typeStr = (type == GL_VERTEX_SHADER) ? "vertex": "frag";

  shaderID = glCreateShader(type);
  glShaderSource(shaderID, 1, &code, NULL);

  glCompileShader(shaderID);

  glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
  if(!success){
    glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
    LOG_ERROR << "Compiling shader ("<< typeStr  << ") failed! infoLog: " << infoLog;
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


