#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>

Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
  : m_Position(position), m_Rotation(rotation), m_Scale(scale)
{}

glm::mat4 Transform::GetModelMatrix() const{
  glm::mat4 model = glm::mat4(1.0);

  // 1. Translate
  model = glm::translate(model, m_Position);

  // 2. Rotate
  model = glm::rotate(model, m_Rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
  model = glm::rotate(model, m_Rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
  model = glm::rotate(model, m_Rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

  // 3. Scale
  model = glm::scale(model, m_Scale);

  return model;
}

void Transform::Translate(const glm::vec3& offset){
  m_Position += offset;
}

void Transform::Rotate(float angle, const glm::vec3& axis){
  if(axis.x != 0.0) m_Rotation.x += angle * axis.x;
  if(axis.y != 0.0) m_Rotation.y += angle * axis.y;
  if(axis.z != 0.0) m_Rotation.z += angle * axis.z;
}

void Transform::SetRotation(float angle, const glm::vec3& axis) {
  m_Rotation = axis * angle;
}

void Transform::SetScale(const glm::vec3& scale){
  m_Scale = scale;
}
