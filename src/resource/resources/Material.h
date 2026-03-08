#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include "glm/glm.hpp"
#include "glm/mat4x4.hpp"

#include "ShaderData.h"
#include "TextureData.h"
#include "TextureConfig.h"
#include "Defaults.h"
#include "json_common.h"

#include "ServiceLocator.h"
#include "TextureManager.h"

class Material {
private:
  // Prevent copying
  Material(const Material &) = delete;
  Material &operator=(const Material &) = delete;

public:
  Material(std::shared_ptr<ShaderData> _shader = Defaults::getDefaultShader());

  // create an empty material with default shader
  static std::shared_ptr<Material> createDefault();

  void bind(std::shared_ptr<ShaderData> overrideShader = nullptr);
  void setFloat(const std::string &name, float value);
  void setInt(const std::string &name, int value);
  void setVec2(const std::string &name, const glm::vec2 &value);
  void setVec3(const std::string &name, const glm::vec3 &value);
  void setVec4(const std::string &name, const glm::vec4 &value);
  void setTexture(const std::string &name,
                  std::shared_ptr<TextureData> texture);
  void setMat4(const std::string &name, const glm::mat4 &value);
  std::shared_ptr<ShaderData> getShader() const {
    if (!shader)
      return nullptr;
    return shader;
  }
  void setShader(std::shared_ptr<ShaderData> s) { shader = s; }

  bool textureExists(const std::string &name);
  std::shared_ptr<TextureData> getTexture(const std::string& name) const {
    auto it = textures.find(name);
    return it != textures.end() ? it->second : nullptr;
  }

  friend void to_json(json &j, const Material &m);
  friend void from_json(const json &j, Material &m);

private:
  std::shared_ptr<ShaderData> shader;
  std::unordered_map<std::string, float> floats;
  std::unordered_map<std::string, glm::vec2> vec2s;
  std::unordered_map<std::string, glm::vec3> vec3s;
  std::unordered_map<std::string, glm::vec4> vec4s;
  std::unordered_map<std::string, int> ints;
  std::unordered_map<std::string, std::shared_ptr<TextureData>> textures;
  std::unordered_map<std::string, glm::mat4> mat4s;

public:
  glm::vec2 tiling{1.0}, offset{0.0};
};
