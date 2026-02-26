#pragma once

#include <vector>
#include <memory>


#include <glm/glm.hpp>
#include <glad/glad.h>

#include "ShaderData.h"

class LineRenderer {
public:
  struct Line {
    glm::vec3 a, b;
    glm::vec3 color;
  };

  static void Initialize(const std::shared_ptr<ShaderData> shader);
  static void Destroy();
private:
  static std::vector<Line> mLines;
  static GLuint mVAO, mVBO;
  static std::shared_ptr<ShaderData> mShader;

public:
  static void DrawLine(glm::vec3 a, glm::vec3 b, glm::vec3 color = {0, 1, 0});

  static void Flush(const glm::mat4 viewProj);
};
