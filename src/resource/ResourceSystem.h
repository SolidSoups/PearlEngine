#pragma once

#include <iostream>
#include <vector>

#include "ResourceManager.h"
#include "TextureData.h"
#include "ShaderData.h"
#include "MaterialData.h"

class ResourceSystem{
  ResourceManager<TextureTag, TextureData> m_Textures;
  ResourceManager<ShaderTag, ShaderData> m_Shaders;
  ResourceManager<MaterialTag, MaterialData> m_Materials;

public:
  static ResourceSystem& Get(){
    static ResourceSystem instance;
    return instance;
  }

  auto& Textures() { return m_Textures; }
  auto& Shaders() { return m_Shaders; }
  auto& Materials() { return m_Materials; }

  void Destroy(){
    // materials don't need to be destroyed... yet
    std::cout << "ResourceSystem::Destroy() -> Destroying materials" << "\n";
    std::vector<MaterialHandle> materialHandles;
    for(auto& [handle, _] : m_Materials.GetAll()){
      materialHandles.push_back(handle);
    }
    for(auto& handle : materialHandles){
      DestroyMaterial(handle);
    }

    // clean up all textures
    std::cout << "ResourceSystem::Destroy() -> Destroying textures" << "\n";
    std::vector<TextureHandle> textureHandles;
    for(auto& [handle, _] : m_Textures.GetAll()){
      textureHandles.push_back(handle);
    }
    for(auto& handle : textureHandles){
      DestroyTexture(handle);
    }

    // clean up all shaders
    std::cout << "ResourceSystem::Destroy() -> Destroying shaders" << "\n";
    std::vector<ShaderHandle> shaderHandles;
    for(auto& [handle, _] : m_Shaders.GetAll()){
      shaderHandles.push_back(handle);
    }
    for(auto& handle : shaderHandles){
      DestroyShader(handle);
    }
  }
};
