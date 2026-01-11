#pragma once

#include <string>
#include <unordered_map>
#include <memory>

#include "ResourceMacros.h"
#include "glm/glm.hpp"
#include "glm/mat4x4.hpp"

#include "ShaderData.h"
#include "TextureData.h"
#include "Material_Asset.h"
#include "Handle.h"

struct MaterialData {
  RESOURCE_CLASS(MaterialData)
public:
  MaterialData(std::shared_ptr<ShaderData> _shader) : shader(_shader) {}

public:
  ShaderDataHandle shaderHandle;
  std::shared_ptr<ShaderData> shader;
public:
  std::unordered_map<std::string, float> floats;
  std::unordered_map<std::string, glm::vec3> vec3s;
  std::unordered_map<std::string, glm::vec4> vec4s;
  std::unordered_map<std::string, int> ints;
  std::unordered_map<std::string, TextureDataHandle> textureHandles;
  std::unordered_map<std::string, glm::mat4> mat4s;
};
PEARL_DEFINE_RESOURCE(MaterialData)

class ResourceSystem;

ShaderDataHandle MaterialGetShaderHandle(ResourceSystem *rs,
                                         MaterialDataHandle handle);

void BindMaterial(ResourceSystem *rs, MaterialDataHandle handle);
void UnbindMaterial();
void DestroyMaterial(ResourceSystem *rs, MaterialDataHandle handle);

void MaterialSetFloat(ResourceSystem *rs, MaterialDataHandle handle,
                      const std::string &name, float value);
void MaterialSetInt(ResourceSystem *rs, MaterialDataHandle handle,
                    const std::string &name, int value);
void MaterialSetVec3(ResourceSystem *rs, MaterialDataHandle handle,
                     const std::string &name, const glm::vec3 &value);
void MaterialSetVec4(ResourceSystem *rs, MaterialDataHandle handle,
                     const std::string &name, const glm::vec4 &value);
void MaterialSetTexture(ResourceSystem *rs, MaterialDataHandle handle,
                        const std::string &name,
                        const TextureDataHandle &value);
void MaterialSetMat4(ResourceSystem *rs, MaterialDataHandle handle,
                     const std::string &name, const glm::mat4 &value);
