#pragma once

#include <iostream>
#include <vector>

#include "ResourceManager.h"
#include "TextureData.h"
#include "ShaderData.h"
#include "MaterialData.h"
#include "MeshData.h"

#include "Logger.h"

class ResourceSystem{
  ResourceManager<TextureTag, TextureData> m_Textures;
  ResourceManager<ShaderTag, ShaderData> m_Shaders;
  ResourceManager<MaterialTag, MaterialData> m_Materials;
  ResourceManager<MeshTag, MeshData> m_Meshes;

public:
  static ResourceSystem& Get(){
    static ResourceSystem instance;
    return instance;
  }

  auto& Textures() { return m_Textures; }
  auto& Shaders() { return m_Shaders; }
  auto& Materials() { return m_Materials; }
  auto& Meshes() { return m_Meshes; }

  void Destroy(){
    // clean up meshes
    LOG_INFO << "ResourceSystem::Destroy() -> Destroying meshes";
    std::vector<MeshHandle> meshHandles;
    for(auto& [handle, _] : m_Meshes.GetAll()){
      meshHandles.push_back(handle); 
    }
    for(auto& handle : meshHandles){
      DestroyMesh(handle);
    }

    // clean up materials
    LOG_INFO << "ResourceSystem::Destroy() -> Destroying materials";
    std::vector<MaterialHandle> materialHandles;
    for(auto& [handle, _] : m_Materials.GetAll()){
      materialHandles.push_back(handle);
    }
    for(auto& handle : materialHandles){
      DestroyMaterial(handle);
    }

    // clean up all textures
    LOG_INFO << "ResourceSystem::Destroy() -> Destroying textures";
    std::vector<TextureHandle> textureHandles;
    for(auto& [handle, _] : m_Textures.GetAll()){
      textureHandles.push_back(handle);
    }
    for(auto& handle : textureHandles){
      DestroyTexture(handle);
    }

    // clean up all shaders
    LOG_INFO << "ResourceSystem::Destroy() -> Destroying shaders";
    std::vector<ShaderHandle> shaderHandles;
    for(auto& [handle, _] : m_Shaders.GetAll()){
      shaderHandles.push_back(handle);
    }
    for(auto& handle : shaderHandles){
      DestroyShader(handle);
    }
  }
};
