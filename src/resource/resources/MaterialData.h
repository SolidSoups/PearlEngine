#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"
#include "glm/mat4x4.hpp"

#include "ShaderData.h"
#include "TextureData.h"
#include "Handle.h"

struct MaterialData{
  ShaderHandle shaderHandle; 
  std::unordered_map<std::string, float> floats;
  std::unordered_map<std::string, glm::vec3> vec3s;
  std::unordered_map<std::string, glm::vec4> vec4s;
  std::unordered_map<std::string, int> ints;
  std::unordered_map<std::string, TextureHandle> textureHandles;
  std::unordered_map<std::string, glm::mat4> mat4s;
};
struct MaterialTag{};
using MaterialHandle = Handle<MaterialTag>;

MaterialHandle CreateMaterial(ShaderHandle shaderHandle);

ShaderHandle MaterialGetShaderHandle(MaterialHandle handle);

void BindMaterial(MaterialHandle handle);
void UnbindMaterial();
void DestroyMaterial(MaterialHandle handle);

void MaterialSetFloat(MaterialHandle handle, const std::string& name, float value);
void MaterialSetInt(MaterialHandle handle, const std::string& name, int value);
void MaterialSetVec3(MaterialHandle handle, const std::string& name, const glm::vec3& value);
void MaterialSetVec4(MaterialHandle handle, const std::string& name, const glm::vec4& value);
void MaterialSetTexture(MaterialHandle handle, const std::string& name, const TextureHandle& value);
void MaterialSetMat4(MaterialHandle handle, const std::string& name, const glm::mat4& value);
