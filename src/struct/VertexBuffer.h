#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class VertexBuffer {
  private:
    unsigned int m_BufferID = 0;
    unsigned int m_Count = 0;

  public:
    VertexBuffer() = default;
    ~VertexBuffer() { if(m_BufferID != 0) glDeleteBuffers(1, &m_BufferID); }
    VertexBuffer(const VertexBuffer &) = delete;
    VertexBuffer &operator=(const VertexBuffer &) = delete;
    VertexBuffer(VertexBuffer &&other) noexcept : m_BufferID(other.m_BufferID), m_Count(other.m_Count) {
        other.m_BufferID = 0;
        other.m_Count = 0;
    }
    VertexBuffer &operator=(VertexBuffer &&other) noexcept {
        if(this != &other){
            if(m_BufferID != 0) glDeleteBuffers(1, &m_BufferID);
            m_BufferID = other.m_BufferID;
            m_Count = other.m_Count;
            other.m_BufferID = 0;
            other.m_Count = 0;
        }
        return *this;
    }

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
