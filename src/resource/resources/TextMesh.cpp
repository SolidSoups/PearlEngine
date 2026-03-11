#include "TextMesh.h"

#include <immintrin.h>
#include <cstring>

#include <glm/glm.hpp>

#include "Logger.h"

TextMesh::TextMesh(const std::vector<float> &vertices,
                   const std::vector<unsigned int> &indices) {
  // generate buffers
  m_VAO.GenerateVertexArrays();
  m_VAO.Bind();

  // upload 4 floats per vertex
  m_VBO.GenerateVertexBuffers(vertices.data(), vertices.size() / 4, 4);

  // upload triangle indices
  m_EBO.GenerateElementBuffers(indices.data(), indices.size());

  // attribute 0: position (3 floats start at 0)
  m_VAO.DefineAttribute(m_VBO, 0, 2, GL_FLOAT, GL_FALSE,
                        4 * sizeof(float), (void *)0);
  // attribute 1: UV (2 floats offset at 3)
  m_VAO.DefineAttribute(m_VBO, 1, 2, GL_FLOAT, GL_FALSE,
                        4 * sizeof(float),
                        (void *)(2 * sizeof(float)));
}
TextMesh::~TextMesh() {}

void TextMesh::Draw() {
  m_VAO.Bind();
  glDrawElements(GL_TRIANGLES, m_EBO.GetElementCount(), GL_UNSIGNED_INT, 0);
}
