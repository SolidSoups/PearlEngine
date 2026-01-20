#pragma once

#include <glad/glad.h>
#include <cstdint>

class GBuffer{
public:
  GBuffer(uint16_t width, uint16_t height);
  ~GBuffer();

  void bind();
  void unbind();
  void resize(uint16_t width, uint16_t height);

  GLuint GetPositionTexture() const { return m_GPosition; }
  GLuint GetNormalTexture() const { return m_GNormal; }
  GLuint GetAlbedoSpecTexture() const { return m_GAlbedoSpec; }
  GLuint GetDepthTexture() const { return m_DepthTexture; }

private:
  unsigned int m_GBuffer = 0;
  unsigned int m_GPosition = 0;
  unsigned int m_GNormal = 0;
  unsigned int m_GAlbedoSpec = 0;
  unsigned int m_DepthTexture = 0;
};
