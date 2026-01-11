#include "ShaderData.h"
#include "Logger.h"
#include "ResourceSystem.h"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

namespace {
ShaderData* GetShaderData(ResourceSystem* rs, ShaderHandle handle, const char* functionName){
  ShaderData* data = rs->Get(handle);
  if(!data){
    std::cerr << "ShaderData.cpp: " << functionName << ": ShaderData is null" << "\n";
  }
  return data;
}
};

void UseShader(ResourceSystem* rs, ShaderDataHandle handle){
  ShaderData* data = GetShaderData(rs, handle, "UseShader");
  if(!data) return;
  glUseProgram(data->id);
}

void ResetShader(){
  glUseProgram(0);
}

void DestroyShader(ResourceSystem* rs, ShaderDataHandle handle){
  ShaderData* data = GetShaderData(rs, handle, "DestroyShader");
  if(!data) return;

  glDeleteProgram(data->id);
  rs->Destroy(handle);
}

// uniforms

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
    LOG_ERROR << "Shader linking failed!";
    return;
  }

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
    LOG_ERROR << "Compiling shader ("<< type << ") failed!";
    return 0;
  }

  return shaderID;
}

void ShaderSetVec3(ResourceSystem* rs, ShaderDataHandle handle, const char* name, const glm::vec3& value){
  ShaderData* data = GetShaderData(rs, handle, "ShaderSetVec3");
  if(!data) return;
  glUniform3fv(glGetUniformLocation(data->id, name), 1, glm::value_ptr(value));
}
void ShaderSetVec4(ResourceSystem* rs, ShaderDataHandle handle, const char* name, const glm::vec4& value){
  ShaderData* data = GetShaderData(rs, handle, "ShaderSetVec4");
  if(!data) return;
  glUniform4fv(glGetUniformLocation(data->id, name), 1, glm::value_ptr(value));
}
void ShaderSetBool(ResourceSystem* rs, ShaderDataHandle handle, const char* name, bool value){
  ShaderData* data = GetShaderData(rs, handle, "ShaderSetBool");
  if(!data) return;
  glUniform1i(glGetUniformLocation(data->id, name), (int)value);
}
void ShaderSetInt(ResourceSystem* rs, ShaderDataHandle handle, const char* name, int value){
  ShaderData* data = GetShaderData(rs, handle, "ShaderSetInt");
  if(!data) return;
  glUniform1i(glGetUniformLocation(data->id, name), value);
}
void ShaderSetFloat(ResourceSystem* rs, ShaderDataHandle handle, const char* name, float value){
  ShaderData* data = GetShaderData(rs, handle, "ShaderSetFloat");
  if(!data) return;
  glUniform1f(glGetUniformLocation(data->id, name), value);
}
void ShaderSetMatrix4(ResourceSystem* rs, ShaderDataHandle handle, const char* name, const glm::mat4& value){
  ShaderData* data = GetShaderData(rs, handle, "ShaderSetMatrix4");
  if(!data) return;
  glUniformMatrix4fv(glGetUniformLocation(data->id, name), 1, GL_FALSE, glm::value_ptr(value));
}


