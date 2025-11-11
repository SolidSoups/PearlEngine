#pragma once
#include "Transform.h"

#include "MaterialData.h"

class Renderable{ 
public:
  virtual void Render() = 0;
  virtual MaterialHandle GetMaterialHandle() const = 0;
  Transform transform;
};
