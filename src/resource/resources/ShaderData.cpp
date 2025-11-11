#include "ShaderData.h"
#include "ResourceSystem.h"

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

ShaderData* GetShaderData(ShaderHandle handle, const char* functionName){
  ShaderData* data = 
    ResourceSystem::Get().Shaders().Get(handle);
  if(!data){
    std::cerr << "ShaderData.cpp: " << functionName << ": ShaderData is null" << "\n";
  }
  return data;
}
};

ShaderHandle CreateShader(const char* vertexPath, const char* fragmentPath){
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

  // create da handle
  ShaderHandle shaderHandle =
    ResourceSystem::Get().Shaders().Create({shaderObjectID});

  return shaderHandle;
}

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


