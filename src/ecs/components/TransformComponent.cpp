#include "TransformComponent.h"
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 TransformComponent::GetModelMatrix() const {
  glm::mat4 model = glm::mat4(1.0f);

  // translate first
  model = glm::translate(model, position);

  // rotate second
  model =
      glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
  model =
      glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
  model =
      glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

  // scale
  model = glm::scale(model, scale);

  return model;
}

void TransformComponent::LookAt(const glm::vec3 &target) {
  glm::vec3 dir = glm::normalize(target - position);
  float pitch = glm::degrees(atan2(dir.y, sqrt(dir.x * dir.x + dir.z * dir.z)));
  float yaw = glm::degrees(atan2(-dir.x, -dir.z));
  rotation = glm::vec3(pitch, yaw, 0);
}

glm::mat4 TransformComponent::GetRotationMatrixYXZ() const {
  return glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), {0, 1, 0}) *
         glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), {1, 0, 0}) *
         glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), {0, 0, 1});
}
glm::mat4 TransformComponent::GetRotationMatrixXYZ() const {
  return glm::rotate(glm::mat4(1.0f), glm::radians(rotation.x), {1, 0, 0}) *
         glm::rotate(glm::mat4(1.0f), glm::radians(rotation.y), {0, 1, 0}) *
         glm::rotate(glm::mat4(1.0f), glm::radians(rotation.z), {0, 0, 1});
}
glm::vec3 TransformComponent::GetForward() const {
  glm::mat4 rot = GetRotationMatrixYXZ();
  return glm::normalize(glm::vec3(rot * glm::vec4(0, 0, -1, 0)));
}
glm::vec3 TransformComponent::GetRight() const {
  glm::mat4 rot = GetRotationMatrixYXZ();
  return glm::normalize(glm::vec3(rot * glm::vec4(1, 0, 0, 0)));
}
glm::vec3 TransformComponent::GetUp() const {
  glm::mat4 rot = GetRotationMatrixYXZ();
  return glm::normalize(glm::vec3(rot * glm::vec4(0, 1, 0, 0)));
}

void TransformComponent::Translate(const glm::vec3 &offset) {
  position += offset;
}

void TransformComponent::Rotate(float angle, const glm::vec3 &axis) {
  if (axis.x != 0.0)
    rotation.x += angle * axis.x;
  if (axis.y != 0.0)
    rotation.y += angle * axis.y;
  if (axis.z != 0.0)
    rotation.z += angle * axis.z;
}

void TransformComponent::SetPosition(const glm::vec3 &pos) { position = pos; }

void TransformComponent::SetRotation(float angle, const glm::vec3 &axis) {
  rotation = axis * angle;
}

void TransformComponent::SetScale(const glm::vec3 &scl) { scale = scl; }
