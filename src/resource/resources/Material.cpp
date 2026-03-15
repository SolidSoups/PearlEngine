#include "Material.h"
#include "Defaults.h"
#include "Logger.h"

#include "ServiceLocator.h"
#include "TextureManager.h"

Material::Material(std::shared_ptr<ShaderData> _shader) : shader(_shader) {}

std::shared_ptr<Material> Material::createDefault() {
  return std::make_shared<Material>(Defaults::getDefaultShader());
}

void Material::bind(std::shared_ptr<ShaderData> overrideShader) {
  if (!shader) {
    LOG_ERROR << "Material::bind: shader is null";
    return;
  }

  auto activeShader = overrideShader ? overrideShader : shader;
  activeShader->use();

  // Upload all ints
  for (const auto &[name, value] : ints) {
    activeShader->setInt(name.c_str(), value);
  }

  // Upload all floats
  for (const auto &[name, value] : floats) {
    activeShader->setFloat(name.c_str(), value);
  }

  // upload all vec2s
  // set tiling uniform if it doesnt exist
  activeShader->setVec2("tiling", tiling);
  activeShader->setVec2("offset", offset);
  for (const auto &[name, value] : vec2s) {
    activeShader->setVec2(name.c_str(), value);
  }

  // Upload all vec3s
  for (const auto &[name, value] : vec3s) {
    activeShader->setVec3(name.c_str(), value);
  }

  // Upload all vec4s
  for (const auto &[name, value] : vec4s) {
    activeShader->setVec4(name.c_str(), value);
  }

  int textureSlot = 0;

  // upload standard textures with correct defaults
  uploadTexWithDefault("texture_diffuse1", textureSlot++, activeShader, Defaults::getWhiteTexture());
  uploadTexWithDefault("texture_specular1", textureSlot++, activeShader, Defaults::getBlackTexture());
  uploadTexWithDefault("texture_normal1", textureSlot++, activeShader, Defaults::getNormalTexture());
  uploadTex("uHeightMap", textureSlot++, activeShader);
  uploadTex("uDiffuseMap", textureSlot++, activeShader);

  // upload all other textures
  for (auto &[key, tex] : textures) {
    if (key == "texture_diffuse1" or key == "texture_specular1" or
        key == "texture_normal1" or key == "uDiffuseMap" or key == "uHeightMap")
      continue;
    tex->bind(textureSlot);
    activeShader->setInt(key.c_str(), textureSlot++);
  }

  // Upload all matrixes
  for (const auto &[name, matrix4] : mat4s) {
    activeShader->setMatrix4(name.c_str(), matrix4);
  }
}

void Material::uploadTex(const std::string &aUniformName, int aTextureSlot, const std::shared_ptr<ShaderData>& aShader) {
  uploadTexWithDefault(aUniformName, aTextureSlot, aShader, Defaults::getBlackTexture());
}

void Material::uploadTexWithDefault(const std::string &aUniformName, int aTextureSlot,
                                     const std::shared_ptr<ShaderData>& aShader,
                                     std::shared_ptr<TextureData> aDefault) {
  auto it = textures.find(aUniformName);
  std::shared_ptr<TextureData> tex = (it != textures.end() && it->second)
                                         ? it->second
                                         : aDefault;
  tex->bind(aTextureSlot);
  aShader->setInt(aUniformName.c_str(), aTextureSlot);
}

void Material::setFloat(const std::string &name, float value) {
  floats[name] = value;
}

void Material::setInt(const std::string &name, int value) {
  ints[name] = value;
}

void Material::setVec2(const std::string &name, const glm::vec2 &value) {
  vec2s[name] = value;
}

void Material::setVec3(const std::string &name, const glm::vec3 &value) {
  vec3s[name] = value;
}

void Material::setVec4(const std::string &name, const glm::vec4 &value) {
  vec4s[name] = value;
}

void Material::setTexture(const std::string &name,
                          std::shared_ptr<TextureData> texture) {
  textures[name] = texture;
}

void Material::setMat4(const std::string &name, const glm::mat4 &value) {
  mat4s[name] = value;
}

bool Material::textureExists(const std::string &name) {
  auto it = textures.find(name);
  return it != textures.end() && it->second.get();
}

// serialization

void to_json(json &j, const Material &m) {
  // serialize uniforms
  j["floats"] = m.floats;
  j["ints"] = m.ints;
  j["vec2s"] = m.vec2s;
  j["vec3s"] = m.vec3s;
  j["vec4s"] = m.vec4s;
  j["mat4s"] = m.mat4s;

  // serialize tiling and offset
  j["tiling"] = m.tiling;
  j["offset"] = m.offset;

  // serialize textures
  for (const auto &[name, tex] : m.textures) {
    j["textures"][name] = *tex;
  }
}
void from_json(const json &j, Material &m) {
  // deserialize uniforms
  m.floats = j["floats"];
  m.ints = j["ints"];
  m.vec2s = j["vec2s"];
  m.vec3s = j["vec3s"];
  m.vec4s = j["vec4s"];
  m.mat4s = j["mat4s"];

  // deserialize tiling and offset
  m.tiling = j["tiling"];
  m.offset = j["offset"];

  // deserialize textures
  if (j.contains("textures")) {
    auto &texManager = ServiceLocator::Get<TextureManager>();
    for (const auto &[key, value] : j["textures"].items()) {
      std::string path = value["filePath"];
      TextureConfig config = value["config"];
      m.textures[key] = texManager.load(path.c_str(), config);
    }
  }
}
