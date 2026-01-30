#pragma once
#include "CameraComponent.h"
#include "CameraData.h"
#include "ServiceHandle.h"
#include "ServiceLocator.h"
#include "Scene.h"
#include "ecs_common.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
  Camera();

  void SetAspect(float aspect);
  CameraData *GetPreview();
  CameraData *GetCurrentTarget();

  CameraData &GetInternal() { return m_InternalCameraData; }

private:
  ServiceHandle<Scene> r_Scene;
  CameraData m_InternalCameraData;
  float m_Aspect = 60.f;

private:
  bool m_IsPreviewingSceneCamera = false;
  ecs::Entity m_PreviewTarget = ecs::NULL_ENTITY;

public:
  inline bool IsPreviewing() const { return m_IsPreviewingSceneCamera; }
  void StartPreview(ecs::Entity cameraEntity);
  void StopPreview();
};
