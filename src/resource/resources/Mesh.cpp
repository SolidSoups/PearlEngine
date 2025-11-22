#include "Mesh.h"

Mesh::Mesh(const std::vector<float> &vertices,
           const std::vector<unsigned int> &indices) {
    m_VAO.GenerateVertexArrays();
    m_VAO.Bind();

    m_VBO.GenerateVertexBuffers(vertices.data(),
                                vertices.size(), 8);
    m_EBO.GenerateElementBuffers(indices.data(),
                                 indices.size() * sizeof(unsigned int));

    m_VAO.AddBuffer(m_VBO, 0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                    (void *)0);
    m_VAO.AddBuffer(m_VBO, 1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                    (void *)(3 * sizeof(float)));
    m_VAO.AddBuffer(m_VBO, 2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                    (void *)(5 * sizeof(float)));
}
Mesh::~Mesh(){
       
}

void Mesh::Draw(){
    m_VAO.Bind();
    glDrawElements(GL_TRIANGLES, m_VBO.GetVertexCount(), GL_UNSIGNED_INT, 0);
}
