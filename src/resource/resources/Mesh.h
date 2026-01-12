#pragma once

#include <vector>

#include "ElementBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

class Mesh {
  // Prevent copying
  Mesh(const Mesh&) = delete;
  Mesh& operator=(const Mesh&) = delete;

  private:
    VertexArray m_VAO;
    VertexBuffer m_VBO;
    ElementBuffer m_EBO;

  public:
    Mesh(const std::vector<float> &vertices,
         const std::vector<unsigned int> &indices);

    Mesh(Mesh &&) noexcept = default;
    Mesh &operator=(Mesh &&) noexcept = default;

    ~Mesh();
    void Draw();
};
