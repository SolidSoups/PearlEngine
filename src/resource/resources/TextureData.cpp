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
#define GET_TEXTURE_OR_NULL(handle) GetTextureData(handle, __func__)
TextureData* GetTextureData(TextureHandle handle, const char* functionName){
  TextureData* data = ResourceSystem::Get().Textures().Get(handle);
  if(!data) 
    LOG_INFO << "TextureData.cpp: " << functionName << ": TextureData is null";
  return data;
}
};

void BindTexture(TextureHandle handle, unsigned int slot){
  TextureData* textureData = GET_TEXTURE_OR_NULL(handle);
  if(!textureData) return;

  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, textureData->id);
}

void UnbindTexture(){
  glBindTexture(GL_TEXTURE_2D, 0);
}

void DestroyTexture(TextureHandle handle){
  TextureData* textureData = GET_TEXTURE_OR_NULL(handle);
  if(!textureData) return;

  glDeleteTextures(1, &textureData->id); 
  ResourceSystem::Get().Textures().Destroy(handle);
}
