#pragma once

#include "Renderable.h"
#include "Material.h"


class Mesh : public Renderable{
public:
  Mesh(const float* vertices, size_t vertexCount, const unsigned int* indices, size_t indexCount);

  void Render();
  
  void SetMaterial(Material* material);
  Material* GetMaterial() const { return m_Material; }
private:
  Material* m_Material = nullptr;

private:
  unsigned int VAO, VBO, EBO;
  size_t m_VertexCount, m_IndexCount;
};
