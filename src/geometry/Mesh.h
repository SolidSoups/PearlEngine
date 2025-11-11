#pragma once

#include "Renderable.h"
#include "MeshData.h"
#include "MaterialData.h"

class Mesh : public Renderable{
public:
  Mesh(const float* vertices, size_t vertexCount, const unsigned int* indices, size_t indexCount);
  ~Mesh();

  void Render();

public:
  void SetMaterial(MaterialHandle handle);
  MaterialHandle GetMaterialHandle() const { return MeshGetMaterial(m_MeshHandle); }

private:
  MeshHandle m_MeshHandle;

};
