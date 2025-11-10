#include "TextureData.h"

// lib
#include <GLFW/glfw3.h>
#include <stb_image.h>

// std
#include <iostream>

TextureData LoadTexture(const std::string& filepath, bool generateMipMaps){
  // set flip flag (global, uses OpenGL coordinates)
  stbi_set_flip_vertically_on_load(true); 

  // load image using stbi
  int width = -1;
  int height = -1;
  int channels = -1;
  unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);

  // check if data couldn't be loaded
  if(!data){
    std::cerr << "TextureData.h: LoadTexture: stbi_load return NULL:\n"
              << "Reason: " << stbi_failure_reason() << "\n";
    return {};
  }

  // add check on size
  if(width > 512 || height > 512){
    std::cout << "TextureData.h: LoadTexture: Image size "
              << width << "x" << height 
              << " is exceeding. OpenGL may not be able to handle such large textures" << "\n";
  }
  
  // Determine OpenGL data formats
  GLenum internalFormat = 0, dataFormat = 0;
  switch(channels){
    case 1:
      internalFormat = GL_RED;
      dataFormat = GL_RED;
      break;
    case 2:
      internalFormat = GL_RGB8;
      dataFormat = GL_RGB;
      break;
    case 4:
      internalFormat = GL_RGBA8;
      dataFormat = GL_RGBA;
      break;
    default:
      std::cerr << "TextureData.h: LoadTexture: Unsupported channel count: "
                << channels << "\n";
      stbi_image_free(data);
      return {};
  }

  // create the TextureData struct
  TextureData textureData{(uint32_t)width, (uint32_t)height, (uint32_t)channels};

  // create OpenGL texture object
  glGenTextures(1, &textureData.textureID);
  glBindTexture(GL_TEXTURE_2D, textureData.textureID);

  // set texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // upload pixel data
  glTexImage2D(GL_TEXTURE_2D,    // Target
               0,                // Mipmap level (0=base)
               internalFormat,   // GPU format (GL_RGB8, GL_RGBA8)
               width,          // Width
               height,         // Height
               0,                // Border (must be 0)
               dataFormat,       // CPU data format (GL_RGB, GL_RGBA)
               GL_UNSIGNED_BYTE, // Data type (8-but per channel)
               data              // Pointer to pixel data
  );

  // generate mip maps
  if(generateMipMaps){
    glGenerateMipmap(GL_TEXTURE_2D);
  }

  // unbind
  glBindTexture(GL_TEXTURE_2D, 0);

  // free CPU memory (GPU has a copy of data now)
  stbi_image_free(data);

  std::cout << "TextureData.h: LoadTexture: Loaded texture: " << filepath << " (" << width << "x"
            << height << ", " << channels << " channels)" << std::endl;

  return textureData;
}


void BindTexture(TextureData texData, unsigned int slot){
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, texData.textureID);
}

void UnbindTexture(){
  glBindTexture(GL_TEXTURE_2D, 0);
}

