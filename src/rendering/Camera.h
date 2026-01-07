#pragma once
#include "CameraData.h"
#include "ServiceHandle.h"
#include "ServiceLocator.h"
#include "Scene.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class Camera{
public:
  Camera();

  glm::mat4 GetViewMatrix() const;
  glm::mat4 GetProjectionMatrix() const;

  void SetPosition(glm::vec3& position);
  void SetTarget(glm::vec3& target);
  void SetAspectRatio(float aspectRatio);

  glm::vec3 GetPosition() const { return m_CameraData.position; }
  glm::vec3 GetTarget() const { return m_CameraData.target; }
  glm::vec3 GetForward() const { return m_CameraData.GetForward(); }
  glm::vec3 GetRight() const { return m_CameraData.GetRight(); }
  glm::vec3 GetUp() const { return m_CameraData.GetUp(); }
  float GetAspectRatio() const { return m_CameraData.aspectRatio; }

  void Translate(glm::vec3& offset);
  void Move(float forward, float right, float up);

  inline CameraData& GetCameraData() { return m_CameraData; }

private:
  ServiceHandle<Scene> r_Scene;
  CameraData m_CameraData;

  void SyncFromComponent();
public:
  void Update();

};
