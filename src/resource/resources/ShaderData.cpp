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

void UseShader(ResourceSystem* rs, ShaderHandle handle){
  ShaderData* data = GetShaderData(rs, handle, "UseShader");
  if(!data) return;
  glUseProgram(data->id);
}

void ResetShader(){
  glUseProgram(0);
}

void DestroyShader(ResourceSystem* rs, ShaderHandle handle){
  ShaderData* data = GetShaderData(rs, handle, "DestroyShader");
  if(!data) return;

  glDeleteProgram(data->id);
  rs->Destroy(handle);
}

// uniforms

void ShaderSetVec3(ResourceSystem* rs, ShaderHandle handle, const char* name, const glm::vec3& value){
  ShaderData* data = GetShaderData(rs, handle, "ShaderSetVec3");
  if(!data) return;
  glUniform3fv(glGetUniformLocation(data->id, name), 1, glm::value_ptr(value));
}
void ShaderSetVec4(ResourceSystem* rs, ShaderHandle handle, const char* name, const glm::vec4& value){
  ShaderData* data = GetShaderData(rs, handle, "ShaderSetVec4");
  if(!data) return;
  glUniform4fv(glGetUniformLocation(data->id, name), 1, glm::value_ptr(value));
}
void ShaderSetBool(ResourceSystem* rs, ShaderHandle handle, const char* name, bool value){
  ShaderData* data = GetShaderData(rs, handle, "ShaderSetBool");
  if(!data) return;
  glUniform1i(glGetUniformLocation(data->id, name), (int)value);
}
void ShaderSetInt(ResourceSystem* rs, ShaderHandle handle, const char* name, int value){
  ShaderData* data = GetShaderData(rs, handle, "ShaderSetInt");
  if(!data) return;
  glUniform1i(glGetUniformLocation(data->id, name), value);
}
void ShaderSetFloat(ResourceSystem* rs, ShaderHandle handle, const char* name, float value){
  ShaderData* data = GetShaderData(rs, handle, "ShaderSetFloat");
  if(!data) return;
  glUniform1f(glGetUniformLocation(data->id, name), value);
}
void ShaderSetMatrix4(ResourceSystem* rs, ShaderHandle handle, const char* name, const glm::mat4& value){
  ShaderData* data = GetShaderData(rs, handle, "ShaderSetMatrix4");
  if(!data) return;
  glUniformMatrix4fv(glGetUniformLocation(data->id, name), 1, GL_FALSE, glm::value_ptr(value));
}


