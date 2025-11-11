#include "MaterialData.h"

#include <iostream>
#include "ResourceSystem.h"

namespace{
MaterialData* GetMaterialData(MaterialHandle handle, const char* functionName){
  MaterialData* data = 
    ResourceSystem::Get().Materials().Get(handle);
  if(!data){
    std::cerr << "MaterialData.cpp: " << functionName << ": MaterialData is null" << "\n";
  }
  return data;
}
};

// TODO: add a null check for the shader handle
MaterialData CreateMaterial(ShaderHandle shaderHandle){
  return {shaderHandle};
}

ShaderHandle MaterialGetShaderHandle(MaterialHandle handle){
  MaterialData* materialData = GetMaterialData(handle, "MaterialGetShaderHandle");
  if(!materialData) return {};
  
  return materialData->shaderHandle;
}

void BindMaterial(MaterialHandle handle){
  MaterialData* materialData = GetMaterialData(handle, "BindMaterial");
  if(!materialData) return;

  ShaderHandle shaderHandle = materialData->shaderHandle;

  UseShader(shaderHandle);

  // Upload all ints
  for(const auto& [name, value] : materialData->ints){
    ShaderSetInt(shaderHandle, name.c_str(), value);
  }

  // Upload all floats
  for(const auto& [name, value] : materialData->floats){
    ShaderSetFloat(shaderHandle, name.c_str(), value);
  }

  // Upload all vec3s
  for(const auto& [name, value] : materialData->vec3s){
    ShaderSetVec3(shaderHandle, name.c_str(), value);
  }

  // Upload all vec4s
  for(const auto& [name, value] : materialData->vec4s){
    ShaderSetVec4(shaderHandle, name.c_str(), value);
  }

  // Upload all textures
  int textureSlot = 0;
  for(const auto& [name, textureHandle] : materialData->textureHandles){
    BindTexture(textureHandle, textureSlot);
    ShaderSetInt(shaderHandle, name.c_str(), textureSlot);
    textureSlot++;
  }

  // Upload all matrixes
  for(const auto& [name, matrix4] : materialData->mat4s){
    ShaderSetMatrix4(shaderHandle, name.c_str(), matrix4);
  }
}

void DestroyMaterial(MaterialHandle handle){
  // nothing for now...
  ResourceSystem::Get().Materials().Destroy(handle);
}

void MaterialSetFloat(MaterialHandle handle, const std::string& name, float value)
{ 
  MaterialData* materialData = GetMaterialData(handle, "MaterialSetFloat");
  if(!materialData) return;
  materialData->floats[name] = value;
}

void MaterialSetInt(MaterialHandle handle, const std::string& name, int value)
{ 
  MaterialData* materialData = GetMaterialData(handle, "MaterialSetInt");
  if(!materialData) return;
  materialData->ints[name] = value; 
}

void MaterialSetVec3(MaterialHandle handle, const std::string& name, const glm::vec3& value) 
{ 
  MaterialData* materialData = GetMaterialData(handle, "MaterialSetVec3");
  if(!materialData) return;
  materialData->vec3s[name] = value; 
}

void MaterialSetVec4(MaterialHandle handle, const std::string& name, const glm::vec4& value) 
{ 
  MaterialData* materialData = GetMaterialData(handle, "MaterialSetVec4");
  if(!materialData) return;
  materialData->vec4s[name] = value; 
}

void MaterialSetTexture(MaterialHandle handle, const std::string& name, const TextureHandle& value)
{ 
  MaterialData* materialData = GetMaterialData(handle, "MaterialSetTexture");
  if(!materialData) return;
  materialData->textureHandles[name] = value;
}

void MaterialSetMat4(MaterialHandle handle, const std::string& name, const glm::mat4& value)
{ 
  MaterialData* materialData = GetMaterialData(handle, "MaterialSetMat4");
  if(!materialData) return;
  materialData->mat4s[name] = value; 
}

