#pragma once

#include <glad/glad.h>

class ElementBuffer {
  private:
    unsigned int m_BufferID = -1;
    unsigned int m_Count = -1;

  public:
    ElementBuffer() = default;
    ~ElementBuffer() { glDeleteBuffers(1, &m_BufferID); }
    ElementBuffer(const ElementBuffer &) = delete;
    ElementBuffer &operator=(const ElementBuffer &) = delete;

  public:
    void GenerateElementBuffers(const unsigned int *data, unsigned int count) {
        m_Count = count;

        glGenBuffers(1, &m_BufferID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int),
                     data, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    void Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferID); }
    GLuint GetCount() const { return m_Count; }
};
