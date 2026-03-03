#include "EngineCamera.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Logger.h"


bool EngineCamera::MoveCamera() {
  const glm::vec2 &mouseDelta = mInputMan.GetMouseDelta();
  float scrollDelta = mInputMan.GetScrollDelta();
  if (mInputMan.GetMouseKey(GLFW_MOUSE_BUTTON_MIDDLE)) {
    Pan(mouseDelta);
    return true;
  }

  bool hasMoved = false;
  if (mInputMan.GetMouseKey(GLFW_MOUSE_BUTTON_RIGHT)) {
    Orbit(mouseDelta);
    hasMoved = true;
  }

  if (scrollDelta != 0.0f) {
    Zoom(scrollDelta);
  }
  return hasMoved;
}

void EngineCamera::CopyEntity(TransformComponent& transform, CameraComponent& camera) {
  glm::vec3 forward = transform.GetForward();

  mPosition = transform.position;
  mOrbitDistance = 5.0f;
  mOrbitTarget = mPosition + forward * mOrbitDistance;

  mPitch = glm::degrees(asinf(-forward.y)); 
  mYaw = glm::degrees(atan2f(-forward.z, -forward.x));
}

void EngineCamera::Reset() {
  mPosition = glm::vec3{5.0f};
  mOrbitTarget = glm::vec3{0.0f, 0.0f, 0.0f};

  glm::vec3 forward = mOrbitTarget - mPosition;
  mOrbitDistance = glm::length(forward);
  forward = glm::normalize(forward);

  mPitch = glm::degrees(asinf(-forward.y));
  mYaw = glm::degrees(atan2f(-forward.z, -forward.x));
}
const glm::mat4 EngineCamera::GetViewMatrix() {
  return glm::lookAt(mPosition, mOrbitTarget, getUp());
}
const glm::mat4 EngineCamera::GetProjectionMatrix(float aspect) {
  return glm::perspective(glm::radians(mFov), aspect, mNearPlane, mFarPlane);
}

const glm::vec3 &EngineCamera::GetPosition() { return mPosition; }

// movement api
void EngineCamera::Pan(glm::vec2 delta) {
  // get camera basis vectors
  glm::vec3 right = getRight();
  glm::vec3 up = getUp();

  // calculate offset
  glm::vec3 offset =
      (-right * delta.x + up * delta.y) * PanSensitivity;

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

  UpdateData();
}
void EngineCamera::Zoom(float scrollDelta) {
  // proportional zoom: step scales with distance
  float zoomAmount = scrollDelta * ZoomSpeed * mOrbitDistance;
  float newDist = mOrbitDistance - zoomAmount;

  // dolly-through: when zooming past the minimum, push the orbit target
  // forward instead of stopping, letting the camera fly through the pivot
  const float minDist = 0.1f;
  if (newDist < minDist) {
    float overshoot = minDist - newDist;
    mOrbitTarget += getForward() * overshoot;
    mOrbitDistance = minDist;
  } else {
    mOrbitDistance = newDist;
  }

  UpdateData();
}

void EngineCamera::UpdateData() {
  // calculate new camera position using spherical coordinates
  float yawRad = glm::radians(mYaw);
  float pitchRad = glm::radians(mPitch);

  // update position
  mPosition.x = mOrbitTarget.x + mOrbitDistance * cos(pitchRad) * cos(yawRad);
  mPosition.y = mOrbitTarget.y + mOrbitDistance * sin(pitchRad);
  mPosition.z = mOrbitTarget.z + mOrbitDistance * cos(pitchRad) * sin(yawRad);
}
