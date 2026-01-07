#pragma once

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"

struct CameraData {
  CameraData() { UpdateCameraVectors(); }

  glm::mat4 GetViewMatrix() { return glm::lookAt(position, target, GetUp()); }
  glm::mat4 GetProjectionMatrix() const {
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane,
                            farPlane);
  }

  void UpdateCameraVectors() {
    forward = glm::normalize(target - position);
    right = glm::normalize(glm::cross(forward, worldUp));
    up = glm::normalize(glm::cross(right, forward));
  }

  // camera position and orientation
  glm::vec3 position = glm::vec3(0.0f, 0.0f, 5.0f);
  glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);

  // perspective params
  float fov = 60.f; // in degrees
  float aspectRatio = 1.0f;

  // frustrum params
  float nearPlane = 0.1f;
  float farPlane = 100.0f;

  inline glm::vec3 &GetForward() {
    forward = glm::normalize(target - position);
    return forward;
  }
  inline glm::vec3 &GetRight() {
    right = glm::normalize(glm::cross(forward, worldUp));
    return forward;
  }
  inline glm::vec3 &GetUp() {
    up = glm::normalize(glm::cross(right, forward));
    return up;
  }

private:
  // camera basis vectors
  glm::vec3 forward;
  glm::vec3 right;
  glm::vec3 up;
};
