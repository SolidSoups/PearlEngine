#pragma once
#include "glad/glad.h"

#include <string>

class Texture{
public:
  Texture(const std::string& filepath, bool generateMipmaps = true);
  ~Texture();

  Texture(const Texture&) = delete;
  Texture& operator=(const Texture&) = delete;

  void Bind(unsigned int slot) const;
  void Unbind() const;
public:
  GLuint GetID() const { return m_TextureID; }
  int GetWidth() const { return m_Width; }
  int GetHeight() const { return m_Height; }
  int GetChannels() const { return m_Channels; }
  const std::string& GetFilepath() const { return m_Filepath; }

private:
  GLuint m_TextureID = 0;
  std::string m_Filepath;
  int m_Width = 0;
  int m_Height = 0;
  int m_Channels = 0;
};
