#pragma once

#include "ResourceManager.h"
#include "TextureData.h"

class ResourceSystem{
  ResourceManager<TextureTag, TextureData> m_Textures;

public:
  static ResourceSystem& Get(){
    static ResourceSystem instance;
    return instance;
  }

  auto& Textures() { return m_Textures; }
};

