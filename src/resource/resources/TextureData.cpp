#include "TextureData.h"

// lib
#include <algorithm>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

// std
#include <iostream>

// src
#include "Logger.h"

TextureData::TextureData(unsigned char *data, uint32_t width, uint32_t height,
                         uint32_t channels, const TextureConfig& config) {
  if (!data) {
    LOG_ERROR << "data is nullptr";
    return;
  }

  // check size
  int maxSize;
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxSize);
  if (width > maxSize || height > maxSize) {
    LOG_WARNING << "Image size " << width << "x" << height << " is exceeding of"
                << maxSize
                << ". OpenGL may not be able to handle such "
                   "large textures";
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
    return;
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
    // LOG_INFO << "Generated mipmaps. Max level: " << maxLevel;
    // verify each level has data
    for(int i=0; i<maxLevel; ++i){
      GLint width, height;
      glGetTexLevelParameteriv(GL_TEXTURE_2D, i, GL_TEXTURE_WIDTH, &width);
      glGetTexLevelParameteriv(GL_TEXTURE_2D, i, GL_TEXTURE_HEIGHT, &height);
      // LOG_INFO << "Mipmap level " << i << ": width=" << width << ", height=" << height;
    } 
  }

  // unbind
  glBindTexture(GL_TEXTURE_2D, 0);

  this->width = width;
  this->height = height;
  this->channels = channels;
}

// New method-based API implementation
void TextureData::bind(unsigned int slot) {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, id);
}

void TextureData::unbind() { glBindTexture(GL_TEXTURE_2D, 0); }
