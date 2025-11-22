#pragma once

#include "ElementBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include <vector>

class Mesh {
  private:
    VertexArray m_VAO;
    VertexBuffer m_VBO;
    ElementBuffer m_EBO;

  public:
    Mesh(const std::vector<float> &vertices,
         const std::vector<unsigned int> &indices);
    Mesh(const Mesh& ) = delete;
    Mesh &operator=(const Mesh&) = delete;
    ~Mesh();
    void Draw();
};
