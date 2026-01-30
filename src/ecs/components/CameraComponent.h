#pragma once

#include "CameraData.h"

struct CameraComponent {
  CameraData cameraData;
  bool isMainCamera = false;

  CameraComponent() = default;
};
