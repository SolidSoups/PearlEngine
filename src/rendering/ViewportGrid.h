#pragma once
#include <memory>
#include <glm/glm.hpp>
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "ShaderData.h"

class ViewportGrid {
public:
    struct Config {
        float lineSpacing = 1.0f;
        int   halfExtent  = 50;
        glm::vec4 lineColor   = {0.35f, 0.35f, 0.35f, 1.0f};
        glm::vec4 xAxisColor  = {0.8f,  0.15f, 0.15f, 1.0f};
        glm::vec4 zAxisColor  = {0.15f, 0.15f, 0.8f,  1.0f};
    };

    Config config;

    ViewportGrid(std::shared_ptr<ShaderData> shader);

    // Call after changing config.lineSpacing or config.halfExtent
    void Rebuild();

    void Draw(const glm::mat4& view, const glm::mat4& projection);

private:
    std::shared_ptr<ShaderData> m_Shader;
    VertexArray  m_VAO;
    VertexBuffer m_VBO;
    int m_VertexCount = 0;
};
