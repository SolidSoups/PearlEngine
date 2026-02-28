#include "LineRenderer.h"

#include "Geometry.h"

std::shared_ptr<ShaderData> LineRenderer::mShader = nullptr;
std::vector<LineRenderer::Line> LineRenderer::mLines{};
GLuint LineRenderer::mVAO = 0;
GLuint LineRenderer::mVBO = 0;
std::unordered_map<int, std::vector<glm::vec3>> LineRenderer::mSegmentsToLines;
std::vector<glm::vec3> LineRenderer::mLineBoxPoints;
std::vector<glm::vec3> LineRenderer::mCachedUnitHemisphere;

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

  // calculate line box points.
  auto &b = mLineBoxPoints;
  const float h = 0.5f;
  const glm::vec2 r_points[] = {{-h, h}, {-h, -h}, {h, -h}, {h, h}};
  for (int i = 0; i < 4; i++) {
    for (int r = 1; r < 4; r++) {
      glm::vec2 p1 = r_points[r - 1];
      glm::vec2 p2 = r_points[r];

      // determine axis
      switch (i) {
      case 0:
        b.push_back({-h, p1.y, p1.x});
        b.push_back({-h, p2.y, p2.x});
        break;
      case 1:
        b.push_back({p1.y, h, p1.x});
        b.push_back({p2.y, h, p2.x});
        break;
      case 2:
        b.push_back({h, -p1.y, p1.x});
        b.push_back({h, -p2.y, p2.x});
        break;
      case 3:
        b.push_back({-p1.y, -h, p1.x});
        b.push_back({-p2.y, -h, p2.x});
        break;
      }
    }
  }

  CacheUnitHemisphere();
}

void LineRenderer::CacheUnitHemisphere(int segments) {
  // NOTE that this code is innefficient by design
  // run only once at start up, it prioritizes readability rather than
  // efficient computation.

  auto &c = mCachedUnitHemisphere;
  for (int i = 1; i <= segments; i++) {
    float angleA = (float)(i - 1) / segments * glm::two_pi<float>();
    float angleB = (float)(i % segments) / segments * glm::two_pi<float>();

    // first add skirt circle
    c.emplace_back(glm::cos(angleA), 0, glm::sin(angleA));
    c.emplace_back(glm::cos(angleB), 0, glm::sin(angleB));

    if (i <= segments / 2) {
      // add XY half circle
      c.emplace_back(glm::cos(angleA), glm::sin(angleA), 0);
      c.emplace_back(glm::cos(angleB), glm::sin(angleB), 0);

      c.emplace_back(0, glm::sin(angleA), glm::cos(angleA));
      c.emplace_back(0, glm::sin(angleB), glm::cos(angleB));
    }
  }
}

void LineRenderer::DrawLine(glm::vec3 a, glm::vec3 b, glm::vec3 color) {
  mLines.push_back({a, b, color});
}

void LineRenderer::DrawWireBox(glm::vec3 center, glm::vec3 size,
                               glm::vec3 color) {
  auto &b = mLineBoxPoints;
  for (int i = 0; i < b.size(); i += 2) {
    mLines.push_back(
        {center + b[i + 0] * size, center + b[i + 1] * size, color});
  }
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
  auto &points = mSegmentsToLines[segments];
  for (int i = 0; i < points.size(); i += 2) {
    mLines.push_back(
        {center + points[i] * radius, center + points[i + 1] * radius, color});
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

void LineRenderer::DrawWireCapsule(glm::vec3 a, glm::vec3 b, float radius,
                                   glm::vec3 color) {
  constexpr int Segments = 24;

  glm::vec3 axis = glm::normalize(b - a);

  // generate orthonormal basis around axis
  glm::vec3 up = glm::abs(glm::dot(axis, glm::vec3(0, 1, 0))) < 0.99
                     ? glm::vec3(0, 1, 0)
                     : glm::vec3(1, 0, 0);
  glm::vec3 tangent = glm::normalize(glm::cross(up, axis));
  glm::vec3 bitangent = glm::cross(axis, tangent);

  // create inner cylindrical body
  mLines.push_back({a + tangent * radius, b + tangent * radius, color});
  mLines.push_back({a - tangent * radius, b - tangent * radius, color});
  mLines.push_back({a + bitangent * radius, b + bitangent * radius, color});
  mLines.push_back({a - bitangent * radius, b - bitangent * radius, color});

  DrawWireHemisphere(a, radius, -axis, tangent);
  DrawWireHemisphere(b, radius, axis, bitangent);
}

void LineRenderer::DrawWireHemisphere(glm::vec3 center, float radius,
                                      glm::vec3 up, glm::vec3 right,
                                      glm::vec3 color) {
  glm::vec3 forward = glm::cross(right, up);
  size_t size = mCachedUnitHemisphere.size();
  for (int i = 0; i < size; i += 2) {
    glm::vec3 &pa = mCachedUnitHemisphere[i];
    glm::vec3 &pb = mCachedUnitHemisphere[i + 1];

    glm::vec3 wa = center + right * (pa.x * radius) + up * (pa.y * radius) +
                   forward * (pa.z * radius);
    glm::vec3 wb = center + right * (pb.x * radius) + up * (pb.y * radius) +
                   forward * (pb.z * radius);

    mLines.push_back({wa, wb, color});
  }
}
