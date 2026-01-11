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
  return data;
}
};

TextureData::TextureData(const std::vector<unsigned char>& pixelData, uint32_t width, uint32_t height, uint32_t channels, bool generateMipMaps){
  if (width > 1024 || height > 1024) {
    LOG_WARNING << "Image size " << width << "x" << height
      << " is exceeding. OpenGL may not be able to handle such "
      "large textures";
  }

  // Determine OpenGL data formats
  GLenum internalFormat = 0, dataFormat = 0;
  switch (channels) {
  case 1:
    internalFormat = GL_RED;
    dataFormat = GL_RED;
    break;
  case 3:
    internalFormat = GL_RGB8;
    dataFormat = GL_RGB;
    break;
  case 4:
    internalFormat = GL_RGBA8;
    dataFormat = GL_RGBA;
    break;
  default:
    LOG_ERROR << "Unsupported channel count: " << channels;
    return;
  }

  // create OpenGL texture object
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);

  // set texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // upload pixel data
  glTexImage2D(GL_TEXTURE_2D,    // Target
               0,                // Mipmap level (0=base)
               internalFormat,   // GPU format (GL_RGB8, GL_RGBA8)
               width,            // Width
               height,           // Height
               0,                // Border (must be 0)
               dataFormat,       // CPU data format (GL_RGB, GL_RGBA)
               GL_UNSIGNED_BYTE, // Data type (8-but per channel)
               pixelData.data()  // Pointer to pixel data
  );

  // generate mip maps
  if (generateMipMaps) {
    glGenerateMipmap(GL_TEXTURE_2D);
  }

  // unbind
  glBindTexture(GL_TEXTURE_2D, 0);

  this->width = width;
  this->height = height;
  this->channels = channels;
}

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
