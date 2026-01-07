#pragma once

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/glm.hpp"
struct CameraData{
  // camera position and orientation
  glm::vec3 position;
  glm::vec3 target;
  glm::vec3 worldUp;

  // perspective params
  float fov;
  float aspectRatio;

  // frustrum params
  float nearPlane;
  float farPlane;

  glm::mat4 GetViewMatrix() const {
    return glm::lookAt(position, target, up);
  }
  glm::mat4 GetProjectionMatrix() const {
    return glm::perspective(fov, aspectRatio, nearPlane, farPlane);
  }

  void UpdateCameraVectors(){
    forward = glm::normalize(target - position);
    right = glm::normalize(glm::cross(forward, worldUp));
    up = glm::normalize(glm::cross(right, forward));
  }

  inline glm::vec3 GetForward() const { return forward; }
  inline glm::vec3 GetRight() const { return right; }
  inline glm::vec3 GetUp() const { return up; }
private:
  // camera basis vectors
  glm::vec3 forward;
  glm::vec3 right;
  glm::vec3 up;

};
