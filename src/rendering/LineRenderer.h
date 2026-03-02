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
  static std::unordered_map<int, std::vector<glm::vec3>> mSegmentsToLines;
  static std::vector<glm::vec3> mLineBoxPoints;

  // A unit hemisphere at (0,0,0) with basis vectors 
  // up=(0,1,0), right=(1,0,0) and forward=(0,0,1)
  static std::vector<glm::vec3> mCachedUnitHemisphere;

public:
  static void DrawLine(glm::vec3 a, glm::vec3 b, glm::vec3 color = {0, 1, 0});
  static void DrawWireSphere(glm::vec3 center, float radius, glm::vec3 color = {0,1,0}, int segments=32);
  static void DrawWireBox(glm::vec3 center, glm::vec3 size, glm::vec3 color = {0,1,0}, glm::quat rotation = glm::quat(1,0,0,0));
  static void DrawWireCapsule(glm::vec3 a, glm::vec3 b, float radius, glm::vec3 color = {0, 1, 0});
  static void DrawWireHemisphere(glm::vec3 center, float radius, glm::vec3 up, glm::vec3 right, glm::vec3 color = {0, 1, 0});

  static void Flush(const glm::mat4 viewProj);
private:
  // cache a wire sphere with a given segment count
  static void CacheCalcWireSphere(int segments);
  static void CacheUnitHemisphere(int segments = 24);
};
