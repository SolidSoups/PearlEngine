#pragma once

#include "Renderable.h"

class Mesh : public Renderable{
public:
  Mesh(const float* vertices, size_t vertexCount, const unsigned int* indices, size_t indexCount);

  void Render();
  
  void SetMaterial(MaterialHandle handle);
  MaterialHandle GetMaterialHandle() const { return m_MaterialHandle; }
private:
  MaterialHandle m_MaterialHandle;

private:
  unsigned int VAO, VBO, EBO;
  size_t m_VertexCount, m_IndexCount;
};
