#include "Mesh.h"
#include "Logger.h"

#define VERTEX_SIZE 11
Mesh::Mesh(const std::vector<float> &vertices,
           const std::vector<unsigned int> &indices) {
  // generate buffers
  m_VAO.GenerateVertexArrays();
  m_VAO.Bind();

  // upload 11 floats per vertex
  m_VBO.GenerateVertexBuffers(vertices.data(), vertices.size() / VERTEX_SIZE, VERTEX_SIZE);

  // upload triangle indices
  m_EBO.GenerateElementBuffers(indices.data(), indices.size());


  // attribute 0: position (3 floats start at 0)
  m_VAO.DefineAttribute(m_VBO, 0, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(float),
                        (void *)0);
  // attribute 1: UV (2 floats offset at 3)
  m_VAO.DefineAttribute(m_VBO, 1, 2, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(float),
                        (void *)(3 * sizeof(float)));
  // attribute 2: Normal (3 floats offset at 5)
  m_VAO.DefineAttribute(m_VBO, 2, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(float),
                        (void *)(5 * sizeof(float)));

  // attribute 3: Tangent (3 floats offset at 8)
  m_VAO.DefineAttribute(m_VBO, 3, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE * sizeof(float),
                        (void *)(8 * sizeof(float)));

  m_MemorySize = vertices.size() * sizeof(float) + indices.size() * sizeof(unsigned int);
}
Mesh::~Mesh() {}

void Mesh::Draw() {
  m_VAO.Bind();
  glDrawElements(GL_TRIANGLES, m_EBO.GetElementCount(), GL_UNSIGNED_INT, 0);
}
