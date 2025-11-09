#pragma once
#include <glm/glm.hpp>
#include <glad/glad.h>

// lib
#include "Shader.h"

// std
#include <string>
#include <unordered_map>


class Material{
public:
  Material(Shader* shader);

  Shader* GetShader() const { return m_Shader; }
public:
  void SetFloat(const std::string& name, float value);
  void SetVec3(const std::string& name, const glm::vec3& value);
  void SetVec4(const std::string& name, const glm::vec4& value);
  void SetInt(const std::string& name, int value);
  void SetTexture(const std::string& name, GLuint textureID);

public:
  void Bind();
  void Unbind();

private:
  Shader* m_Shader;

  // uniforms
  std::unordered_map<std::string, float> m_Floats;
  std::unordered_map<std::string, glm::vec3> m_Vec3s;
  std::unordered_map<std::string, glm::vec4> m_Vec4s;
  std::unordered_map<std::string, int> m_Ints;
  std::unordered_map<std::string, GLuint> m_Textures;
};
