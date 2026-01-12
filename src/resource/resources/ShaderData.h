#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

struct ShaderData {
  // Prevent copying
  ShaderData(const ShaderData&) = delete;
  ShaderData& operator=(const ShaderData&) = delete;

public:
  ShaderData(const char* vertFile, const char* fragFile);
  ShaderData(GLuint _id) : id(_id) {}

  // New method-based API
  void use();
  void setVec3(const char* name, const glm::vec3& value);
  void setVec4(const char* name, const glm::vec4& value);
  void setBool(const char* name, bool value);
  void setInt(const char* name, int value);
  void setFloat(const char* name, float value);
  void setMatrix4(const char* name, const glm::mat4& value);
  static void reset();

  GLuint id = 0;

private:
  GLuint compileShader(const char* code, unsigned int type);
};

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
