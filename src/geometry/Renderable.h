#pragma once
#include "Transform.h"

class Material;

class Renderable{ 
public:
  virtual void Render() = 0;
  virtual Material* GetMaterial() const = 0;
  Transform transform;
};
