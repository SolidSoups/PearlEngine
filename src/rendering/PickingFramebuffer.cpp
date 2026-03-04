#include "PickingFramebuffer.h"

PickingFramebuffer::PickingFramebuffer(unsigned int width, unsigned int height)
    : mWidth(width), mHeight(height) {
  Create();
}

PickingFramebuffer::~PickingFramebuffer(){
  Destroy();
}

void PickingFramebuffer::Create() {
  glGenFramebuffers(1, &mFBO);
  glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

  // create entity id texture
  glGenTextures(1, &mTexture);
  glBindTexture(GL_TEXTURE_2D, mTexture);
  // generate storage for texture
  glTexImage2D(GL_TEXTURE_2D, 0, GL_R32UI, mWidth, mHeight, 0, GL_RED_INTEGER,
               GL_UNSIGNED_INT, nullptr);

  // set texture filtering
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  // create depth buffer
  glGenRenderbuffers(1, &mDepth);
  glBindRenderbuffer(GL_RENDERBUFFER, mDepth);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mWidth, mHeight);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                            GL_RENDERBUFFER, mDepth);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                         mTexture, 0);

  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    LOG_ERROR << "Framebuffer is incomplete!";
  }

  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void PickingFramebuffer::Destroy() {
  if (mFBO){
    glDeleteFramebuffers(1, &mFBO);
    mFBO = 0;
  }
  if (mTexture){
    glDeleteTextures(1, &mTexture);
    mTexture = 0;
  }
  if (mDepth){
    glDeleteRenderbuffers(1, &mDepth);
    mDepth = 0;
  }
}

void PickingFramebuffer::Resize(unsigned int width, unsigned int height) {
  if (width == mWidth && height == mHeight)
    return;

  mWidth = width;
  mHeight = height;
  Destroy();
  Create();
}

void PickingFramebuffer::Bind(){
  glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
  glViewport(0, 0, mWidth, mHeight);
}

void PickingFramebuffer::Unbind(){
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

