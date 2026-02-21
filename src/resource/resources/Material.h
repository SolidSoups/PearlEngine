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
public:
  struct ConstructData {
    std::unordered_map<std::string, float> floats;
    std::unordered_map<std::string, glm::vec3> vec3s;
    std::unordered_map<std::string, glm::vec4> vec4s;
    std::unordered_map<std::string, int> ints;
    std::unordered_map<std::string, glm::mat4> mat4s;
    std::unordered_map<std::string, std::pair<std::string, TextureConfig>>
        texturePath_config;
  };

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
  void setVec3(const std::string &name, const glm::vec3 &value);
  void setVec4(const std::string &name, const glm::vec4 &value);
  void setTexture(const std::string &name,
                  std::shared_ptr<TextureData> texture);
  void setMat4(const std::string &name, const glm::mat4 &value);
  std::shared_ptr<ShaderData> getShader() const { return shader; }

  bool textureExists(const std::string &name);

  inline const ConstructData createConstruction() const {
    ConstructData data;
    data.floats = floats;
    data.vec3s = vec3s;
    data.vec4s = vec4s;
    data.ints = ints;
    data.mat4s = mat4s;
    for (auto [key, texture] : textures) {
      data.texturePath_config[key] =
          std::make_pair(texture->getFilePath(), texture->getConfig());
    }
    return data;
  }
  inline const void fromConstruction(ConstructData data) {
    floats = data.floats;
    vec3s = data.vec3s;
    vec4s = data.vec4s;
    ints = data.ints;
    mat4s = data.mat4s;
    for (auto &[key, pair] : data.texturePath_config) {
      std::string path = pair.first;
      TextureConfig config = pair.second;
      textures[key] =
          ServiceLocator::Get<TextureManager>().load(path.c_str(), config);
    }
  }

private:
  std::shared_ptr<ShaderData> shader;

  std::unordered_map<std::string, float> floats;
  std::unordered_map<std::string, glm::vec3> vec3s;
  std::unordered_map<std::string, glm::vec4> vec4s;
  std::unordered_map<std::string, int> ints;
  std::unordered_map<std::string, std::shared_ptr<TextureData>> textures;
  std::unordered_map<std::string, glm::mat4> mat4s;
};

inline void to_json(json &j, const Material::ConstructData &m) {
  j["floats"] = m.floats;
  j["ints"] = m.ints;
  j["vec3s"] = m.vec3s;
  j["vec4s"] = m.vec4s;
  j["mat4s"] = m.mat4s;

  // textures
  for (const auto &[key, pair] : m.texturePath_config) {
    j["textures"][key] = {pair.first, pair.second};
  }
}
inline void from_json(const json &j, Material::ConstructData &m) {
  m.floats = j["floats"];
  m.ints = j["ints"];
  m.vec3s = j["vec3s"];
  m.vec4s = j["vec4s"];
  m.mat4s = j["mat4s"];

  if (j.contains("textures")) {
    for (const auto &[key, value] : j["textures"].items()) {
      std::string path = value[0];
      TextureConfig config = value[1];
      m.texturePath_config[key] = std::make_pair(path, config);
    }
  }
}
