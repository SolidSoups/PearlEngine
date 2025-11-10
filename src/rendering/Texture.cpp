#include "Texture.h"

#include <iostream>
#include <unistd.h>

#include "stb_image.h"

Texture::Texture(const std::string &filepath, bool generateMipmaps)
    : m_Filepath(filepath) {

  stbi_set_flip_vertically_on_load(true);

  // Initialize to known values
  m_Width = -1;
  m_Height = -1;
  m_Channels = -1;

  // load image data using stbi
  unsigned char *data =
      stbi_load(filepath.c_str(), &m_Width, &m_Height, &m_Channels, 0);

  // first error check
  if (!data) {
    std::cerr << "Texture::Texture(..) -> ✗ stbi_load returned NULL!" << std::endl;
    std::cerr << "   Reason: " << stbi_failure_reason() << std::endl;
    return;
  }

  // check size
  if(m_Width > 512 && m_Height > 512){
    std::cout << "Texture::Texture(..) -> Image size exceeds 512x512, OpenGL may not be able to handle such large images" << std::endl;
  }

  // Determine OpenGL format based on channels
  GLenum internalFormat = 0;
  GLenum dataFormat = 0;
  switch (m_Channels) {
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
      std::cerr << "Texture::Texture(..) -> Unsupported channel count: "
        << m_Channels << std::endl;
      stbi_image_free(data);
      return;
  }

  // create OpenGL texture object
  glGenTextures(1, &m_TextureID);
  glBindTexture(GL_TEXTURE_2D, m_TextureID);

  // set parameters (wrapping, filtering)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // upload pixel data
  glTexImage2D(GL_TEXTURE_2D,    // Target
               0,                // Mipmap level (0=base)
               internalFormat,   // GPU format (GL_RGB8, GL_RGBA8)
               m_Width,          // Width
               m_Height,         // Height
               0,                // Border (must be 0)
               dataFormat,       // CPU data format (GL_RGB, GL_RGBA)
               GL_UNSIGNED_BYTE, // Data type (8-but per channel)
               data              // Pointer to pixel data
  );

  // generate mipmaps
  if (generateMipmaps) {
    glGenerateMipmap(GL_TEXTURE_2D);
  }

  // unbind
  glBindTexture(GL_TEXTURE_2D, 0);

  // free the CPU memory (GPU has a copy now)
  stbi_image_free(data);

  std::cout << "Texture::Texture(..) -> Loaded texture: " << filepath << " (" << m_Width << "x"
            << m_Height << ", " << m_Channels << " channels)" << std::endl;
}

Texture::~Texture() {
  if (m_TextureID) {
    glDeleteTextures(1, &m_TextureID);
  }
}

void Texture::Bind(unsigned int slot) const {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, m_TextureID);
}

void Texture::Unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }
