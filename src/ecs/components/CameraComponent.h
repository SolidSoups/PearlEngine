#pragma once

#include "CameraData.h"
#include "Component.h"

struct CameraComponent : public IComponent {
  COMPONENT_CLASS(CameraComponent)

public:
  CameraData cameraData;
  bool isMainCamera = false;

  CameraComponent() = default;
};
