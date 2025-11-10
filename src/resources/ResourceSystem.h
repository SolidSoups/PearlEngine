#pragma once

#include "ResourceManager.h"
#include "TextureData.h"
#include "ShaderData.h"

class ResourceSystem{
  ResourceManager<TextureTag, TextureData> m_Textures;
  ResourceManager<ShaderTag, ShaderData> m_Shaders;

public:
  static ResourceSystem& Get(){
    static ResourceSystem instance;
    return instance;
  }

  auto& Textures() { return m_Textures; }
  auto& Shaders() { return m_Shaders; }
};

