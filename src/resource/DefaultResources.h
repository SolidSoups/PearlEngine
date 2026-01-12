#pragma once

#include <memory>

#include "ShaderData.h"
#include "ShaderManager.h"
#include "ServiceLocator.h"

class DefaultResources{
private:
  std::shared_ptr<ShaderData> m_DefaultShader;

public:
  DefaultResources(){
    m_DefaultShader = ServiceLocator::Get<ShaderManager>().load("shaders/vert.glsl", "shaders/frag.glsl");
  }
  std::shared_ptr<ShaderData> getDefaultShader() const { return m_DefaultShader; }
};
