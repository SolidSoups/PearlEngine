#pragma once

#include <glm/glm.hpp>

#include "InputManager.h"

class IEngineCamera {
public:
  IEngineCamera(InputManager &input) : mInputMan(input) {}
  virtual ~IEngineCamera() = default;

  virtual void MoveCamera() = 0;

  virtual const glm::mat4 GetViewMatrix() = 0;
  virtual const glm::mat4 GetProjectionMatrix(float aspect) = 0;

protected:
  InputManager &mInputMan;
};

class EngineCamera : public IEngineCamera {
public:
  EngineCamera(InputManager &input) : IEngineCamera(input) {}

  // disables copy construct/operator
  EngineCamera(EngineCamera &) = delete;
  EngineCamera &operator=(EngineCamera &) = delete;

  // disable movement
  EngineCamera(EngineCamera &&) = delete;
  EngineCamera &&operator=(EngineCamera &&) = delete;

public:
  void MoveCamera() override;
  const glm::mat4 GetViewMatrix() override;
  const glm::mat4 GetProjectionMatrix(float aspect) override;

private:
  // movement api
  void Orbit(glm::vec2 delta);
  void Pan(glm::vec2 delta);
  void Zoom(float scrollDelta);

public:
  // movement settings
  float OrbitSensitivity{0.3f};
  float PanSensitivity{0.001f};
  float ZoomSpeed{0.5f};

private:
  // transform data
  glm::vec3 mPosition;
  const glm::vec3 cWorldUp{0.0f, 1.0f, 0.0f};
  glm::vec3 getForward() const {
    return glm::normalize(mOrbitTarget - mPosition);
  }
  glm::vec3 getRight() const {
    return glm::normalize(glm::cross(getForward(), cWorldUp));
  }
  glm::vec3 getUp() const {
    return glm::normalize(glm::cross(getRight(), getForward()));
  }

  // movement data
  glm::vec3 mOrbitTarget{0.0f, 0.0f, -2.0f};
  float mOrbitDistance{5.0f};
  float mYaw{0.0f};
  float mPitch{0.0f};

  // camera data
  float mFov{60.f};
  float mNearPlane{0.1f};
  float mFarPlane{100.f};
};
