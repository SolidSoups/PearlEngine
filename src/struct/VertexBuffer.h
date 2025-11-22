#pragma once

#include <glad/glad.h>

class VertexBuffer {
  private:
    unsigned int m_BufferID = -1;
    unsigned int m_Count = -1;

  public:
    VertexBuffer() = default;
    ~VertexBuffer() { glDeleteBuffers(1, &m_BufferID); }
    VertexBuffer(const VertexBuffer &) = delete;
    VertexBuffer &operator=(const VertexBuffer &) = delete;

  public:
    void GenerateVertexBuffers(const float* data, unsigned int vertexCount, unsigned int vertexSize){
        m_Count = vertexCount;
        glGenBuffers(1, &m_BufferID);
        glBindBuffer(GL_ARRAY_BUFFER, m_BufferID);
        glBufferData(GL_ARRAY_BUFFER, vertexCount * vertexSize * sizeof(float), data, GL_STATIC_DRAW);
    }
    void Bind() const { glBindBuffer(GL_VERTEX_ARRAY, m_BufferID); }
    inline const unsigned int GetVertexCount() const { return m_Count; }
};
