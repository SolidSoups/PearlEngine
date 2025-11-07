#pragma once

#include "Mesh.h"
#include "Transform.h"


class Cube : public Mesh{
public:
  Cube(); 

  Transform transform;
private:
  static const float s_Vertices[];
  static const unsigned int s_Indices[];
  static const size_t s_VertexCount;
  static const size_t s_IndexCount;
};
