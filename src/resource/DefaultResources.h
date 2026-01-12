#pragma once

#include <memory>

#include "ShaderData.h"
#include "ShaderLoader.h"

class DefaultResources{
private:
  std::shared_ptr<ShaderData> m_DefaultShader;

public:
  DefaultResources(){
    m_DefaultShader = ShaderLoader::load("shaders/vert.glsl", "shaders/frag.glsl");
  }
  std::shared_ptr<ShaderData> getDefaultShader() const { return m_DefaultShader; }
};
