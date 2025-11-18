#pragma once

#include <glm/glm.hpp>

#include "Component.h"

struct TransformComponent : Component{
  COMPONENT_CLASS(TransformComponent)

public:
  glm::vec3 position{0.0f};
  glm::vec3 rotation{0.0f};
  glm::vec3 scale{1.0f};

  // Constructor
  TransformComponent(
    glm::vec3 pos = glm::vec3(0.0f),
    glm::vec3 rot = glm::vec3(0.0f),
    glm::vec3 scl = glm::vec3(1.0f)
  ) : position(pos), rotation(rot), scale(scl) {}

  glm::mat4 GetModelMatrix() const;

  // modifiers
  void Translate(const glm::vec3& offset);
  void Rotate(float angle, const glm::vec3& axis);

  // setters
  void SetRotation(float angle, const glm::vec3& axis);
  void SetPosition(const glm::vec3& position);
  void SetScale(const glm::vec3& scale);
};
