#pragma once

#include <cstddef>
#include <vector>

#include "ElementBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

class Mesh {
private:
  VertexArray m_VAO;
  VertexBuffer m_VBO;
  ElementBuffer m_EBO;

  size_t m_MemorySize = 0;
  std::string mFilePath;
public:
  // Prevent copying
  Mesh(const Mesh &) = delete;
  Mesh &operator=(const Mesh &) = delete;
  Mesh(const std::vector<float> &vertices,
       const std::vector<unsigned int> &indices, const char* filepath = "");

  Mesh(Mesh &&) noexcept = default;
  Mesh &operator=(Mesh &&) noexcept = default;
  ~Mesh();

public:
  void Draw();
  inline size_t getMemorySize() const { return m_MemorySize; }
  const std::string& getFilePath() const { return mFilePath; }
};
