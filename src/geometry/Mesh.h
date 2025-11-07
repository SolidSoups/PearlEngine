#pragma once

#include "Renderable.h"


class Mesh : public Renderable{
public:
  Mesh(const float* vertices, size_t vertexCount, const unsigned int* indices, size_t indexCount);

  void Render(Shader& shader);

private:
  unsigned int VAO, VBO, EBO;
  size_t m_VertexCount, m_IndexCount;
};
