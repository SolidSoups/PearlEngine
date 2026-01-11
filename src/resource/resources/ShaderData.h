#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Handle.h"
#include "ResourceMacros.h"
#include "Shader_Asset.h"

struct ShaderData {
  RESOURCE_CLASS(ShaderData)
public:
  ShaderData(const char* vertFile, const char* fragFile);
  ShaderData(GLuint _id) : id(_id) {}
  GLuint id = 0;

private:
  GLuint compileShader(const char* code, unsigned int type);  
};
PEARL_DEFINE_RESOURCE(ShaderData)

class ResourceSystem;

void UseShader(ResourceSystem *rs, ShaderDataHandle handle);
void ResetShader();
void DestroyShader(ResourceSystem *rs, ShaderDataHandle handle);

void ShaderSetVec3(ResourceSystem *rs, ShaderDataHandle handle,
                   const char *name, const glm::vec3 &value);
void ShaderSetVec4(ResourceSystem *rs, ShaderDataHandle handle,
                   const char *name, const glm::vec4 &value);
void ShaderSetBool(ResourceSystem *rs, ShaderDataHandle handle,
                   const char *name, bool value);
void ShaderSetInt(ResourceSystem *rs, ShaderDataHandle handle, const char *name,
                  int value);
void ShaderSetFloat(ResourceSystem *rs, ShaderDataHandle handle,
                    const char *name, float value);
void ShaderSetMatrix4(ResourceSystem *rs, ShaderDataHandle handle,
                      const char *name, const glm::mat4 &value);

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
