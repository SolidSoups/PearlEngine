#include "ShaderData.h"
#include "Logger.h"
#include "ResourceSystem.h"

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

namespace {
ShaderData* GetShaderData(ShaderHandle handle, const char* functionName){
  ShaderData* data = 
    ResourceSystem::Get().Shaders().Get(handle);
  if(!data){
    std::cerr << "ShaderData.cpp: " << functionName << ": ShaderData is null" << "\n";
  }
  return data;
}
};

void UseShader(ShaderHandle handle){
  ShaderData* data = GetShaderData(handle, "UseShader");
  if(!data) return;
  glUseProgram(data->id);
}

void ResetShader(){
  glUseProgram(0);
}

void DestroyShader(ShaderHandle handle){
  ShaderData* data = GetShaderData(handle, "DestroyShader");
  if(!data) return;

  glDeleteProgram(data->id);
  ResourceSystem::Get().Shaders().Destroy(handle);
}

// uniforms

void ShaderSetVec3(ShaderHandle handle, const char* name, const glm::vec3& value){
  ShaderData* data = GetShaderData(handle, "ShaderSetVec3");
  if(!data) return;
  glUniform3fv(glGetUniformLocation(data->id, name), 1, glm::value_ptr(value));
}
void ShaderSetVec4(ShaderHandle handle, const char* name, const glm::vec4& value){
  ShaderData* data = GetShaderData(handle, "ShaderSetVec4");
  if(!data) return;
  glUniform4fv(glGetUniformLocation(data->id, name), 1, glm::value_ptr(value));
}
void ShaderSetBool(ShaderHandle handle, const char* name, bool value){
  ShaderData* data = GetShaderData(handle, "ShaderSetBool");
  if(!data) return;
  glUniform1i(glGetUniformLocation(data->id, name), (int)value);
}
void ShaderSetInt(ShaderHandle handle, const char* name, int value){
  ShaderData* data = GetShaderData(handle, "ShaderSetInt");
  if(!data) return;
  glUniform1i(glGetUniformLocation(data->id, name), value);
}
void ShaderSetFloat(ShaderHandle handle, const char* name, float value){
  ShaderData* data = GetShaderData(handle, "ShaderSetFloat");
  if(!data) return;
  glUniform1f(glGetUniformLocation(data->id, name), value);
}
void ShaderSetMatrix4(ShaderHandle handle, const char* name, const glm::mat4& value){
  ShaderData* data = GetShaderData(handle, "ShaderSetMatrix4");
  if(!data) return;
  glUniformMatrix4fv(glGetUniformLocation(data->id, name), 1, GL_FALSE, glm::value_ptr(value));
}


