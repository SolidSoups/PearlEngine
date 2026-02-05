#pragma once

#include "json_common.h"
#include "CameraData.h"

struct CameraComponent {
  CameraData cameraData;
  bool isMainCamera = false;

  CameraComponent() = default;
};

inline void to_json(json& j, const CameraComponent& cmp){
  j["fov"] = cmp.cameraData.fov;
  j["near_plane"] = cmp.cameraData.nearPlane;
  j["far_plane"] = cmp.cameraData.farPlane;
}

inline void from_json(const json& j, CameraComponent& cmp){
  cmp.cameraData.fov = j["fov"];
  cmp.cameraData.nearPlane = j["near_plane"];
  cmp.cameraData.farPlane = j["far_plane"];
}
