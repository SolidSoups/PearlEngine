#pragma once

#include "Renderable.h"
#include "MeshData.h"
#include "MaterialData.h"

class Mesh : public Renderable{
public:
  Mesh(const float* vertices, size_t vertexCount, const unsigned int* indices, size_t indexCount);
  ~Mesh();

  void Render();

private:
  MeshHandle m_MeshHandle;

};
