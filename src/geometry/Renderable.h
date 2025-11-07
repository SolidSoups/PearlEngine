#pragma once
#include "Shader.h"

class Renderable{ 
public:
  virtual void Render(Shader& aShader) = 0;
};
