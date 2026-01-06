#include "TextureData.h"

// lib
#include <GLFW/glfw3.h>
#include <stb_image.h>

// std
#include <iostream>

// src
#include "ResourceSystem.h"
#include "Logger.h"

namespace{
#define GET_TEXTURE_OR_NULL(rs, handle) GetTextureData(rs, handle, __func__)
TextureData* GetTextureData(ResourceSystem* rs, TextureHandle handle, const char* functionName){
  TextureData* data = rs->Get(handle);
  if(!data)
    LOG_INFO << "TextureData.cpp: " << functionName << ": TextureData is null";
  return data;
}
};

void BindTexture(ResourceSystem* rs, TextureHandle handle, unsigned int slot){
  TextureData* textureData = GET_TEXTURE_OR_NULL(rs, handle);
  if(!textureData) return;

  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, textureData->id);
}

void UnbindTexture(){
  glBindTexture(GL_TEXTURE_2D, 0);
}

void DestroyTexture(ResourceSystem* rs, TextureHandle handle){
  TextureData* textureData = GET_TEXTURE_OR_NULL(rs, handle);
  if(!textureData) return;

  glDeleteTextures(1, &textureData->id);
  rs->Destroy(handle);
}
