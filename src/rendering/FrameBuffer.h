#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// GLuint is an unsigned int, why do we use it? idk...

class Framebuffer{
public:
  Framebuffer(unsigned int width, unsigned int height);
  ~Framebuffer();

  void Bind();
  void Unbind();
  void Resize(unsigned int width, unsigned int height);

  GLuint GetFBOId() const { return m_FBO; }
  GLuint GetTextureID() const { return m_ColorTexture; }
  unsigned int GetWidth() const { return m_Width; }
  unsigned int GetHeight() const { return m_Height; }

private:
  void Create();
  void Destroy();

  GLuint m_FBO = 0;
  GLuint m_ColorTexture = 0;
  GLuint m_DepthStencilRBO = 0;
  unsigned int m_Width, m_Height;
};
