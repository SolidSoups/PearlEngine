#pragma once

#include "ElementBuffer.h"
#include "VertexBuffer.h"

class VertexArray {
  private:
    unsigned int m_ArrayID = -1;

  public:
    VertexArray() = default;
    ~VertexArray() { glDeleteVertexArrays(1, &m_ArrayID); }
    VertexArray(const VertexArray &) = delete;
    VertexArray &operator=(const VertexArray &) = delete;

    void GenerateVertexArrays() { glGenVertexArrays(1, &m_ArrayID); }
    void Bind() const { glBindVertexArray(m_ArrayID); }

    void AddBuffer(const VertexBuffer &vb, unsigned int index, int size,
                   GLenum type, bool normalized, int stride,
                   const void *offset) {
        Bind();
        vb.Bind();
        glVertexAttribPointer(index, size, type, normalized, stride, offset);
        glEnableVertexAttribArray(index);
    }
};
