#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class ElementBuffer {
  private:
    unsigned int m_BufferId = 0;
    unsigned int m_Count = 0;

  public:
    ElementBuffer() = default;
    ~ElementBuffer() { if(m_BufferId != 0) glDeleteBuffers(1, &m_BufferId); }
    ElementBuffer(const ElementBuffer &) = delete;
    ElementBuffer &operator=(const ElementBuffer &) = delete;
    ElementBuffer(ElementBuffer &&other) noexcept : m_BufferId(other.m_BufferId), m_Count(other.m_Count) {
        other.m_BufferId = 0;
        other.m_Count = 0;
    }
    ElementBuffer &operator=(ElementBuffer&& other) noexcept {
        if(this != &other){
            if(m_BufferId != 0) glDeleteBuffers(1, &m_BufferId);
            m_BufferId = other.m_BufferId;
            m_Count = other.m_Count;
            other.m_BufferId = 0;
            other.m_Count = 0;
        }
        return *this;
    }

  public:
    void GenerateElementBuffers(const unsigned int *data, unsigned int count) {
        m_Count = count;

        glGenBuffers(1, &m_BufferId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int),
                     data, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    void Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferId); }
    GLuint GetCount() const { return m_Count; }
};
