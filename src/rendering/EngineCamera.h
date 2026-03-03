#pragma once

#include <glm/glm.hpp>

#include "ecs_coordinator.h"
#include "ecs_common.h"
#include "InputManager.h"

#include "TransformComponent.h"
#include "CameraComponent.h"

class IEngineCamera {
public:
  IEngineCamera(InputManager &input) : mInputMan(input) {}
  virtual ~IEngineCamera() = default;

  virtual bool MoveCamera() = 0;
  virtual void Reset() = 0;
  virtual void CopyEntity(TransformComponent& transform, CameraComponent& camera){}
  virtual const glm::mat4 GetViewMatrix() = 0;
  virtual const glm::mat4 GetProjectionMatrix(float aspect) = 0;
  virtual const glm::vec3& GetPosition() = 0;

protected:
  InputManager &mInputMan;
};

class EngineCamera : public IEngineCamera {
public:
  EngineCamera(InputManager &input) : IEngineCamera(input) {
    Reset();
  }

  // disables copy construct/operator
  EngineCamera(EngineCamera &) = delete;
  EngineCamera &operator=(EngineCamera &) = delete;

  // disable movement
  EngineCamera(EngineCamera &&) = delete;
  EngineCamera &&operator=(EngineCamera &&) = delete;

public:
  bool MoveCamera() override;
  void Reset() override;
  void CopyEntity(TransformComponent& transform, CameraComponent& camera) override;
  const glm::mat4 GetViewMatrix() override;
  const glm::mat4 GetProjectionMatrix(float aspect) override;
  const glm::vec3 &GetPosition() override;

private:
  // movement api
  void Orbit(glm::vec2 delta);
  void Pan(glm::vec2 delta);
  void Zoom(float scrollDelta);
  void UpdateData();

public:
  // movement settings
  float OrbitSensitivity{0.3f};
  float PanSensitivity{0.05f};
  float ZoomSpeed{0.1f};

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
  float mFarPlane{1000.f};
};
