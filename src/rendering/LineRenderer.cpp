#include "LineRenderer.h"

std::shared_ptr<ShaderData> LineRenderer::mShader = nullptr;
std::vector<LineRenderer::Line> LineRenderer::mLines{};
GLuint LineRenderer::mVAO = 0;
GLuint LineRenderer::mVBO = 0;
std::unordered_map<int, std::vector<glm::vec3>>
    LineRenderer::mSegmentsToLines;

void LineRenderer::Initialize(const std::shared_ptr<ShaderData> shader) {
  mShader = shader;

  // initalize objects
  glGenVertexArrays(1, &mVAO);
  glGenBuffers(1, &mVBO);
  glBindVertexArray(mVAO);
  glBindBuffer(GL_ARRAY_BUFFER, mVBO);

  // position
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);

  // color
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                        (void *)(3 * sizeof(float)));
}

void LineRenderer::DrawLine(glm::vec3 a, glm::vec3 b, glm::vec3 color) {
  mLines.push_back({a, b, color});
}

// how am i supposed to cache this? radius + segments at center in world?
void LineRenderer::DrawWireSphere(glm::vec3 center, float radius,
                                  glm::vec3 color, int segments) {
  // calculate wire sphere if we haven't yet
  auto it = mSegmentsToLines.find(segments);
  if (it == mSegmentsToLines.end()) {
    CacheCalcWireSphere(segments);
  }

  // safe to index now...
  auto& points = mSegmentsToLines[segments];
  for(int i=0; i<points.size(); i+=2){
    mLines.push_back({
      center + points[i] * radius,
      center + points[i+1] * radius,
      color
    });
  }
}

void LineRenderer::CacheCalcWireSphere(int segments) {
  float tps = glm::two_pi<float>() / segments;

  std::vector<glm::vec3> points;
  // for every plane ...
  for (int p = 0; p < 3; p++) {
    // ... and for every segment ...
    for (int seg = 1; seg <= segments; seg++) {
      // calculate angles
      float a0 = (float)((seg - 1) % segments) * tps;
      float a1 = (float)(seg % segments) * tps;

      // calculate arm lengths
      float c0 = glm::cos(a0);
      float s0 = glm::sin(a0);
      float c1 = glm::cos(a1);
      float s1 = glm::sin(a1);

      // push back lines
      switch (p) {
      case 0:
        points.push_back(glm::vec3(c0, s0, 0));
        points.push_back(glm::vec3(c1, s1, 0));
        break;
      case 1:
        points.push_back(glm::vec3(c0, 0, s0));
        points.push_back(glm::vec3(c1, 0, s1));
        break;
      case 2:
        points.push_back(glm::vec3(0, s0, c0));
        points.push_back(glm::vec3(0, s1, c1));
        break;
      }
    }
  }

  mSegmentsToLines[segments] = std::move(points);
}

void LineRenderer::Flush(const glm::mat4 viewProj) {
  if (mVAO == 0 || mVBO == 0)
    return;
  if (mLines.empty())
    return;

  // interleave: a, b, color
  std::vector<float> verts;
  for (auto &l : mLines) {
    verts.insert(verts.end(),
                 {l.a.x, l.a.y, l.a.z, l.color.r, l.color.g, l.color.b});
    verts.insert(verts.end(),
                 {l.b.x, l.b.y, l.b.z, l.color.r, l.color.g, l.color.b});
  }

  glBindVertexArray(mVAO);
  glBindBuffer(GL_ARRAY_BUFFER, mVBO);
  glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(),
               GL_DYNAMIC_DRAW);

  mShader->use();
  mShader->setMatrix4("uViewProj", viewProj);

  glDrawArrays(GL_LINES, 0, mLines.size() * 2);

  mLines.clear();
}

void LineRenderer::Destroy() {
  // delete objects if used
  if (mVAO)
    glDeleteVertexArrays(1, &mVAO);
  if (mVBO)
    glDeleteBuffers(1, &mVBO);
}
