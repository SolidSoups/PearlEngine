#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include "glm/glm.hpp"
#include "glm/mat4x4.hpp"

#include "ShaderData.h"
#include "TextureData.h"

struct Material {
  // Prevent copying
  Material(const Material&) = delete;
  Material& operator=(const Material&) = delete;

public:
  Material(std::shared_ptr<ShaderData> _shader);

  // create an empty material with default shader
  static std::shared_ptr<Material> createDefault();

  void bind();
  void setFloat(const std::string& name, float value);
  void setInt(const std::string& name, int value);
  void setVec3(const std::string& name, const glm::vec3& value);
  void setVec4(const std::string& name, const glm::vec4& value);
  void setTexture(const std::string& name, std::shared_ptr<TextureData> texture);
  void setMat4(const std::string& name, const glm::mat4& value);
  std::shared_ptr<ShaderData> getShader() const { return shader; }

  bool textureExists(const std::string& name);


private:
  std::shared_ptr<ShaderData> shader;

  std::unordered_map<std::string, float> floats;
  std::unordered_map<std::string, glm::vec3> vec3s;
  std::unordered_map<std::string, glm::vec4> vec4s;
  std::unordered_map<std::string, int> ints;
  std::unordered_map<std::string, std::shared_ptr<TextureData>> textures;
  std::unordered_map<std::string, glm::mat4> mat4s;
};
