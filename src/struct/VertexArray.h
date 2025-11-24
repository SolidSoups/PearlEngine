#pragma once

#include "ElementBuffer.h"
#include "VertexBuffer.h"

class VertexArray {
  private:
    unsigned int m_ArrayID = 0;

  public:
    VertexArray() = default;
    ~VertexArray() { if(m_ArrayID != 0) glDeleteVertexArrays(1, &m_ArrayID); }
    VertexArray(const VertexArray &) = delete;
    VertexArray &operator=(const VertexArray &) = delete;
    VertexArray(VertexArray &&other) noexcept : m_ArrayID(other.m_ArrayID) {
        other.m_ArrayID = 0;
    }
    VertexArray &operator=(VertexArray &&other) noexcept {
        if(this != &other){
            if(m_ArrayID != 0) glDeleteVertexArrays(1, &m_ArrayID);
            m_ArrayID = other.m_ArrayID;
            other.m_ArrayID = 0;
        }
        return *this;
    }

    void GenerateVertexArrays() { glGenVertexArrays(1, &m_ArrayID); }
    void Bind() const { glBindVertexArray(m_ArrayID); }

    void DefineAttribute(const VertexBuffer &vb, unsigned int index, int size,
                   GLenum type, bool normalized, int stride,
                   const void *offset) {
        Bind();
        vb.Bind();
        glVertexAttribPointer(index, size, type, normalized, stride, offset);
        glEnableVertexAttribArray(index);
    }
};
