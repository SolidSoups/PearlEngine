#pragma once

#include "CameraData.h"
#include "Component.h"

struct CameraComponent : Component {
  COMPONENT_CLASS(CameraComponent)

public:
  CameraData cameraData;

  CameraComponent() = default;
};
