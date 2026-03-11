#pragma once

#include <cstddef>
#include <vector>

#include "ElementBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

class TextMesh {
private:
  VertexArray m_VAO;
  VertexBuffer m_VBO;
  ElementBuffer m_EBO;
public:
  // Prevent copying
  TextMesh(const std::vector<float> &vertices,
       const std::vector<unsigned int> &indices);
  TextMesh(const TextMesh &) = delete;
  TextMesh &operator=(const TextMesh &) = delete;

  TextMesh(TextMesh &&) noexcept = default;
  TextMesh &operator=(TextMesh &&) noexcept = default;
  ~TextMesh();

public:
  void Draw();
};
