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
  for(const auto &[name, value] : vec2s) {
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

  // upload all other textures
  for(auto& [key, tex] : textures){
    if(key == "texture_diffuse1" or key == "texture_specular1" or key == "texture_normal1")
      continue;
    tex->bind(textureSlot);    
    activeShader->setInt(key.c_str(), textureSlot++);
  }

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

void Material::setVec2(const std::string &name, const glm::vec2 &value){
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

void to_json(json& j, const Material& m){
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
  for(const auto& [name, tex] : m.textures){
    j["textures"][name] = *tex;
  }
}
void from_json(const json& j, Material& m){
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
    auto& texManager = ServiceLocator::Get<TextureManager>();
    for(const auto& [key, value] : j["textures"].items()) {
      std::string path = value["filePath"];
      TextureConfig config = value["config"];
      // if (value.is_array()) {
      //   path = value[0];
      //   config = value[1];
      // } else if (value.contains("path")) {
      //   path = value["path"];
      //   config = value["config"];
      // } else {
      //   path = value["filePath"];
      //   config = value["config"];
      // }
      m.textures[key] = texManager.load(path.c_str(), config);
    }
  }
}
