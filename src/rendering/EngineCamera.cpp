#include "EngineCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void EngineCamera::MoveCamera() {
  const glm::vec2 &mouseDelta = mInputMan.GetMouseDelta();
  float scrollDelta = mInputMan.GetScrollDelta();
  if (mInputMan.GetMouseKey(GLFW_MOUSE_BUTTON_MIDDLE)) {
    Pan(mouseDelta);
    return;
  }

  if (mInputMan.GetMouseKey(GLFW_MOUSE_BUTTON_RIGHT)) {
    Orbit(mouseDelta);
  }

  if (scrollDelta != 0) {
    Zoom(scrollDelta);
  }
}
const glm::mat4 EngineCamera::GetViewMatrix() {
  return glm::lookAt(mPosition, mOrbitTarget, getUp());
}
const glm::mat4 EngineCamera::GetProjectionMatrix(float aspect) {
  return glm::perspective(glm::radians(mFov), aspect, mNearPlane, mFarPlane);
}

// movement api
void EngineCamera::Pan(glm::vec2 delta) {
  // get camera basis vectors
  glm::vec3 right = getRight();
  glm::vec3 up = getUp();

  // calculate offset
  glm::vec3 offset =
      (-right * delta.x + up * delta.y) * PanSensitivity * mOrbitDistance;

  // translate by offset
  mPosition += offset;
  mOrbitTarget += offset;
}
void EngineCamera::Orbit(glm::vec2 delta) {
  // update yaw and pitch
  mYaw += delta.x * OrbitSensitivity;
  mPitch += delta.y * OrbitSensitivity;

  // clamp pitch to avoid gimal lock
  mPitch = glm::clamp(mPitch, -89.f, 89.f);

  // calculate new camera position using spherical coordinates
  float yawRad = glm::radians(mYaw);
  float pitchRad = glm::radians(mPitch);

  mPosition.x = mOrbitTarget.x + mOrbitDistance * cos(pitchRad) * cos(yawRad);
  mPosition.y = mOrbitTarget.y + mOrbitDistance * sin(pitchRad);
  mPosition.z = mOrbitTarget.z + mOrbitDistance * cos(pitchRad) * sin(yawRad);
}
void EngineCamera::Zoom(float scrollDelta) {
  mOrbitDistance -= scrollDelta * ZoomSpeed;
  mOrbitDistance = glm::clamp(mOrbitDistance, 0.5f, 50.f);
}
