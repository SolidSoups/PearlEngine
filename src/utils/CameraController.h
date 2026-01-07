#pragma once

#include "Camera.h"

class CameraController{
public:
  CameraController(Camera* camera);

  void OnUpdate(glm::vec2 mouseDelta, float scrollDelta, bool rightMouseDown, bool middleMouseDown);

  void SetOrbitTarget(const glm::vec3& target) { m_OrbitTarget = target; }
  void SetOrbitDistance(float distance) { m_OrbitDistance = distance; }

  float GetOrbitSensitivity() const { return m_OrbitSensitivity; }
  void SetOrbitSensitivity(float sensitivity) { m_OrbitSensitivity = sensitivity; }

  float GetPanSensitivity() const { return m_PanSensitivity; }
  void SetPanSensitivity(float sensitivity) { m_PanSensitivity = sensitivity; }

  void Reset();

private:
  void Orbit(glm::vec2 delta);
  void Pan(glm::vec2 delta);
  void Zoom(float scrollDelta);

  void UpdateCameraPosition();
  void RecalculateOrbitFromCamera();

  Camera* m_Camera;
  glm::vec3 m_OrbitTarget{0.0f, 0.0f, -2.0f};
  float m_OrbitDistance = 5.0f;
  float m_Yaw = 0.0f;
  float m_Pitch = 0.0f;

  float m_OrbitSensitivity = 0.3f;
  float m_PanSensitivity = 0.001f;
  float m_ZoomSpeed = 0.5f;
};
