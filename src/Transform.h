#pragma once
#include <glm/glm.hpp>

class Transform {
public:
  Transform(glm::vec3 position = glm::vec3(0.0f),
            glm::vec3 rotation = glm::vec3(0.0f),
            glm::vec3 scale = glm::vec3(1.0f));

  glm::mat4 GetModelMatrix() const;

  void Translate(const glm::vec3& offset);
  void Rotate(float angle, const glm::vec3& axis);
  void SetRotation(float angle, const glm::vec3& axis);
  void SetScale(const glm::vec3& scale);

private:
  glm::vec3 m_Position;
  glm::vec3 m_Rotation;
  glm::vec3 m_Scale;
};
