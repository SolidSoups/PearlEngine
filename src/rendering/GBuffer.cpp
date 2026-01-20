#include "GBuffer.h"

#include <stdexcept>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

GBuffer::GBuffer(uint16_t width, uint16_t height){
  glGenFramebuffers(1, &m_GBuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, m_GBuffer);

  // position color buffer
  glGenTextures(1, &m_GPosition);
  glBindTexture(GL_TEXTURE_2D, m_GPosition);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, (GLsizei)width, (GLsizei)height, 0, GL_RGBA, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_GPosition, 0);

  // normal color buffer
  glGenTextures(1, &m_GNormal);
  glBindTexture(GL_TEXTURE_2D, m_GNormal);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, (GLsizei)width, (GLsizei)height, 0, GL_RGBA, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_GNormal, 0);
  
  // color + specular color buffer 
  glGenTextures(1, &m_GAlbedoSpec);
  glBindTexture(GL_TEXTURE_2D, m_GAlbedoSpec);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)width, (GLsizei)height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_GAlbedoSpec, 0);

  // tell opengl which color attachments we'll use (of this framebuffer) for rendering
  unsigned int attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
  glDrawBuffers(3, attachments);

  // depth texture
  glGenTextures(1, &m_DepthTexture);
  glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, (GLsizei)width, (GLsizei)height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture, 0);

  // completeness check
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
    throw std::runtime_error("GBuffer is incomplete");
  }

  // unbind 
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GBuffer::~GBuffer(){
	if(m_GPosition != 0)   glDeleteTextures(1, &m_GPosition);
	if(m_GNormal != 0)   glDeleteTextures(1, &m_GNormal);
	if(m_GAlbedoSpec != 0)   glDeleteTextures(1, &m_GAlbedoSpec);
	if(m_DepthTexture != 0)   glDeleteTextures(1, &m_DepthTexture);
	if(m_GBuffer != 0)   glDeleteFramebuffers(1, &m_GBuffer);
}


void GBuffer::bind(){
  glBindFramebuffer(GL_FRAMEBUFFER, m_GBuffer);  
}

void GBuffer::unbind(){
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GBuffer::resize(uint16_t width, uint16_t height){
  glBindTexture(GL_TEXTURE_2D, m_GPosition);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);

  glBindTexture(GL_TEXTURE_2D, m_GNormal);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);

  glBindTexture(GL_TEXTURE_2D, m_GAlbedoSpec);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

  glBindTexture(GL_TEXTURE_2D, m_DepthTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

}
