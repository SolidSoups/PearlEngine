#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <memory>
#include "ShaderData.h"

class PickingFramebuffer {
public:
  PickingFramebuffer(unsigned int width, unsigned int height);
  ~PickingFramebuffer();

  void Resize(unsigned int width, unsigned int height);
  void Bind();
  void Unbind();
private:
  void Create();
  void Destroy();
private:
  std::shared_ptr<ShaderData> mShader;

  GLuint mFBO = 0;
  GLuint mDepth = 0;
  GLuint mTexture = 0;
  unsigned int mWidth, mHeight;
};
