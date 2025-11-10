#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Handle.h"

struct ShaderData {
  GLuint id = 0;
};
struct ShaderTag {};
using ShaderHandle = Handle<ShaderTag>;

ShaderData CreateShader(const char *vertexPath, const char *fragmentPath);
void UseShader(ShaderData shaderData);
void ResetShader();
void DestroyShader(ShaderData shaderData);

void ShaderSetVec3(ShaderData shaderData, const char *name,
                   const glm::vec3 &value);
void ShaderSetVec4(ShaderData shaderData, const char *name,
                   const glm::vec4 &value);
void ShaderSetBool(ShaderData shaderData, const char *name, bool value);
void ShaderSetInt(ShaderData shaderData, const char *name, int value);
void ShaderSetFloat(ShaderData shaderData, const char *name,
                    float value);
void ShaderSetMatrix4(ShaderData shaderData, const char *name,
                      const glm::mat4 &value);



// TODO: Performance Optimization - Uniform Location Caching
// Currently calling glGetUniformLocation() on every uniform set, which is a
// slow lookup operation. For frequently-set uniforms (MVP matrices, material
// properties, etc.), consider implementing:
//
// struct ShaderData {
//   GLuint id = 0;
//   std::unordered_map<std::string, GLint> uniformCache;  // Cache uniform
//   locations
// };
//
// Helper function:
// GLint GetCachedUniformLocation(ShaderData& shader, const char* name) {
//   auto it = shader.uniformCache.find(name);
//   if (it != shader.uniformCache.end()) return it->second;
//   GLint loc = glGetUniformLocation(shader.id, name);
//   shader.uniformCache[name] = loc;
//   return loc;
// }
//
// Then use: glUniform3fv(GetCachedUniformLocation(shader, name), 1, value);
// This will significantly improve performance for uniforms set every frame.
