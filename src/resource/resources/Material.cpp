#include "Material.h"
#include "Defaults.h"
#include "Logger.h"
#include "ServiceLocator.h"

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

  // Upload all vec3s
  for (const auto &[name, value] : vec3s) {
    activeShader->setVec3(name.c_str(), value);
  }

  // Upload all vec4s
  for (const auto &[name, value] : vec4s) {
    activeShader->setVec4(name.c_str(), value);
  }

  int textureSlot = 0;

  // upload diffuse
  auto albedo = textures.find("texture_diffuse1");
  std::shared_ptr<TextureData> albedoTex =
      (albedo != textures.end() && albedo->second)
          ? albedo->second
          : Defaults::getWhiteTexture();
  albedoTex->bind(textureSlot);
  activeShader->setInt("texture_diffuse1", textureSlot++);

  // upload specular
  auto spec = textures.find("texture_specular1");
  std::shared_ptr<TextureData> specTex =
      (spec != textures.end() && spec->second) ? spec->second
                                               : Defaults::getBlackTexture();
  specTex->bind(textureSlot);
  activeShader->setInt("texture_specular1", textureSlot++);

  // upload normals
  auto normal = textures.find("texture_normal1");
  std::shared_ptr<TextureData> normalTex =
    (normal != textures.end() && normal->second) ? normal->second
                                                 : Defaults::getNormalTexture();
  normalTex->bind(textureSlot);
  activeShader->setInt("texture_normal1", textureSlot++);

  // Upload all matrixes
  for (const auto &[name, matrix4] : mat4s) {
    activeShader->setMatrix4(name.c_str(), matrix4);
  }
}

void Material::setFloat(const std::string &name, float value) {
  floats[name] = value;
}

void Material::setInt(const std::string &name, int value) {
  ints[name] = value;
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
