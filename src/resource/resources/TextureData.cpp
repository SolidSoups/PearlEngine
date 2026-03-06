#include "TextureData.h"

// std
#include <iostream>
// lib
#include <algorithm>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
// src
#include "Logger.h"

TextureData::~TextureData(){
  if(id != 0) glDeleteTextures(1, &id); 
}

TextureData::TextureData(TextureData && other){
  id = other.id;
  other.id = 0;
  filePath = other.filePath;
  config = other.config;
  width = other.width;
  height = other.height;
  channels = other.channels;
}
TextureData& TextureData::operator=(TextureData && other){
  if(this == &other) return *this;
  if(id != 0) glDeleteTextures(1, &id);
  id = other.GetTextureID();
  other.id = 0;
  filePath = std::move(other.filePath);
  config = other.config;
  width = other.width;
  height = other.height;
  channels = other.channels;
  return *this;
}

void TextureData::setConfig(const TextureConfig &aConfig){
  config = aConfig;
}

bool TextureData::loadFile(const char* path){
  // set flip flag (global, uses OpenGL coordinates)
  stbi_set_flip_vertically_on_load(true);

  // load image using stbi
  int width = -1;
  int height = -1;
  int channels = -1;
  unsigned char *data = stbi_load(path, &width, &height, &channels, 0);

  // check if data couldn't be loaded
  if (!data) {
    LOG_ERROR << "stbi_load return NULL:\n"
              << "Reason: " << stbi_failure_reason();
    return false;
  }

  // store result for later (we need to delete the image data before we do anything after)
  bool ok = loadData(data, width, height, channels);

  // free image data (we don't need it anymore)
  stbi_image_free(data);
  
  return ok;
}
bool TextureData::loadData(unsigned char* data, uint32_t width, uint32_t height, uint32_t channels){
  if(!data){
    LOG_ERROR << "Data is null";
    return false;
  }

  // check size and log warning
  int maxSize;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize);
  if (width > maxSize || height > maxSize) {
    LOG_WARNING << "Image size " << width << "x" << height << " is exceeding of"
                << maxSize
                << ". OpenGL may not be able to handle such large textures";
  }

  // warning threshold
  if (width > 2048 || height > 2048) {
    LOG_WARNING << "Large texture: " << width << "x" << height << " ("
                << (width * height * channels / 1024 / 1024) << " MB)";
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
    return false;
  }

  // create OpenGL texture object
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_2D, id);

  // set texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, config.wrapS);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, config.wrapT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, config.minFilter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, config.magFilter);

  // LOD parameters
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, config.lodBias);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_LOD, config.minLod);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_LOD, config.maxLod);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, config.maxMipMapLevel);

  // antisotropic filtering (if supported)
  if(config.antisotropicLevel > 1){
    GLfloat maxAnisotropy;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY, &maxAnisotropy);
    GLfloat aniso = std::min((float)config.antisotropicLevel, maxAnisotropy);
    glTexParameterf(GL_TEXTURE_2D, GL_MAX_TEXTURE_MAX_ANISOTROPY, aniso);
  }

  // upload pixel data
  glTexImage2D(GL_TEXTURE_2D,    // Target
               0,                // Mipmap level (0=base)
               internalFormat,   // GPU format (GL_RGB8, GL_RGBA8)
               width,            // Width
               height,           // Height
               0,                // Border (must be 0)
               dataFormat,       // CPU data format (GL_RGB, GL_RGBA)
               GL_UNSIGNED_BYTE, // Data type (8-bit per channel)
               data              // Pointer to pixel data
  );

  // generate mip maps
  if (config.generateMipMaps) {
    glGenerateMipmap(GL_TEXTURE_2D);
    
    // check if mipmap exists
    GLint maxLevel;
    glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, &maxLevel);
    LOG_INFO << "Generated mipmaps. Max level: " << maxLevel;
    // verify each level has data
    for(int i=0; i<maxLevel; ++i){
      GLint width, height;
      glGetTexLevelParameteriv(GL_TEXTURE_2D, i, GL_TEXTURE_WIDTH, &width);
      glGetTexLevelParameteriv(GL_TEXTURE_2D, i, GL_TEXTURE_HEIGHT, &height);
      LOG_INFO << "Mipmap level " << i << ": width=" << width << ", height=" << height;
    } 
  }


  // unbind
  glBindTexture(GL_TEXTURE_2D, 0);
  
  this->width = width;
  this->height = height;
  this->channels = channels;
  return true;
}

// New method-based API implementation
void TextureData::bind(unsigned int slot) {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, id);
}

void TextureData::unbind() { glBindTexture(GL_TEXTURE_2D, 0); }
