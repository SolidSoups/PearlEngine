#include "FrameBuffer.h"
#include "Logger.h"
#include <algorithm>
#include <iostream>


Framebuffer::Framebuffer(unsigned int width, unsigned int height)
  : m_Width(width)
  , m_Height(height)
{
  Create();
}

Framebuffer::~Framebuffer(){
  Destroy();
}

void Framebuffer::Create(){
  // Generate frame buffer 
  glGenFramebuffers(1, &m_FBO);
  glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

  // Create color texture attachment
  glGenTextures(1, &m_ColorTexture);
  glBindTexture(GL_TEXTURE_2D, m_ColorTexture);
  //    generate storage for the texture
  glTexImage2D(
    GL_TEXTURE_2D, 0, GL_RGB, 
    m_Width, m_Height, 
    0, GL_RGB, GL_UNSIGNED_BYTE,
    nullptr
  );
  //    set texture filtering to linear interpolation for both minification and magnification (smooth scaling)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  //    attach the texture to the frame buffers first color attachment (0), this is where shader frag output will be written
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTexture, 0);

  // Create the depth and stencil render buffer
  glGenRenderbuffers(1, &m_DepthStencilRBO);
  glBindRenderbuffer(GL_RENDERBUFFER, m_DepthStencilRBO);
  //    allocates storage with a combined depth of 32B (24 for depth, 8 for stencil)
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
  //    attach the renderbuffer to the frame buffer's depth-stencil attachment point for depth testing and stencil operations.
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthStencilRBO);

  // Check framebuffer completeness
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
    LOG_INFO << "FrameBuffer::Create() -> Framebuffer is not complete";
  }

  glBindTexture(GL_TEXTURE_2D, 0);
  glBindRenderbuffer(GL_RENDERBUFFER, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Destroy(){
  if(m_FBO) glDeleteFramebuffers(1, &m_FBO);
  if(m_ColorTexture) glDeleteTextures(1, &m_ColorTexture);
  if(m_DepthStencilRBO) glDeleteRenderbuffers(1, &m_DepthStencilRBO);
}

void Framebuffer::Bind(){
  glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
  glViewport(0, 0, m_Width, m_Height);
}

void Framebuffer::Unbind(){
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::Resize(unsigned int width, unsigned int height){
  if(width == m_Width && height == m_Height) return;

  m_Width = width;
  m_Height = height;
  Destroy();
  Create();
}




