#include "Mesh.h"
#include "Logger.h"

Mesh::Mesh(const std::vector<float> &vertices,
           const std::vector<unsigned int> &indices) {
  // generate buffers
  m_VAO.GenerateVertexArrays();
  m_VAO.Bind();

  m_VBO.GenerateVertexBuffers(vertices.data(), vertices.size() / 8, 8);
  m_EBO.GenerateElementBuffers(indices.data(), indices.size());

  // define attribute
  m_VAO.DefineAttribute(m_VBO, 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)0);
  m_VAO.DefineAttribute(m_VBO, 1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  m_VAO.DefineAttribute(m_VBO, 2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(5 * sizeof(float)));

  m_MemorySize = vertices.size() * sizeof(float) + indices.size() * sizeof(unsigned int);
}
Mesh::~Mesh() {}

void Mesh::Draw() {
  m_VAO.Bind();
  glDrawElements(GL_TRIANGLES, m_EBO.GetElementCount(), GL_UNSIGNED_INT, 0);
}
