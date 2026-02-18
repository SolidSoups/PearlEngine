#include "ViewportGrid.h"
#include <glad/glad.h>
#include <vector>
#include <cmath>

ViewportGrid::ViewportGrid(std::shared_ptr<ShaderData> shader)
    : m_Shader(std::move(shader))
{
    m_VAO.GenerateVertexArrays();
    Rebuild();
}

void ViewportGrid::Rebuild() {
    // Each vertex: x, y, z, axisType  (4 floats)
    std::vector<float> data;

    const float ext = static_cast<float>(config.halfExtent);
    const float step = config.lineSpacing;

    // Lines parallel to X axis (constant Z, vary X)
    for (float z = -ext; z <= ext + 0.001f; z += step) {
        float axisType = (std::fabs(z) < step * 0.01f) ? 1.0f : 0.0f;
        data.insert(data.end(), { -ext, 0.0f, z, axisType });
        data.insert(data.end(), {  ext, 0.0f, z, axisType });
    }

    // Lines parallel to Z axis (constant X, vary Z)
    for (float x = -ext; x <= ext + 0.001f; x += step) {
        float axisType = (std::fabs(x) < step * 0.01f) ? 2.0f : 0.0f;
        data.insert(data.end(), { x, 0.0f, -ext, axisType });
        data.insert(data.end(), { x, 0.0f,  ext, axisType });
    }

    m_VertexCount = static_cast<int>(data.size() / 4);

    m_VBO = VertexBuffer{};
    m_VBO.GenerateVertexBuffers(data.data(), m_VertexCount, 4);

    m_VAO.DefineAttribute(m_VBO, 0, 3, GL_FLOAT, false, 4 * sizeof(float), (void*)0);
    m_VAO.DefineAttribute(m_VBO, 1, 1, GL_FLOAT, false, 4 * sizeof(float), (void*)(3 * sizeof(float)));
}

void ViewportGrid::Draw(const glm::mat4& view, const glm::mat4& projection) {
    m_Shader->use();
    m_Shader->setMatrix4("view", view);
    m_Shader->setMatrix4("projection", projection);
    m_Shader->setVec4("lineColor",  config.lineColor);
    m_Shader->setVec4("xAxisColor", config.xAxisColor);
    m_Shader->setVec4("zAxisColor", config.zAxisColor);

    m_VAO.Bind();
    glDrawArrays(GL_LINES, 0, m_VertexCount);
}
