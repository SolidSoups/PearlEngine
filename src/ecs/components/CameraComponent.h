#pragma once

#include "json_common.h"
#include "CameraData.h"

struct CameraComponent {
  float fov = 60.0f;
  float aspectModifier = 1.0f;
  float nearPlane = 0.1f;
  float farPlane = 100.0f;

  // Runtime state — not serialized
  CameraData cameraData; // no no no
  bool isMainCamera = false;
  inline bool IsMainCamera() const { return isMainCamera; }
};

inline void to_json(json &j, const CameraComponent &cmp) {
  j["fov"] = cmp.fov;
  j["aspectRatio"] = cmp.aspectModifier;
  j["near_plane"] = cmp.nearPlane;
  j["far_plane"] = cmp.farPlane;
}

inline void from_json(const json &j, CameraComponent &cmp) {
  cmp.fov = j["fov"];
  cmp.aspectModifier = j["aspectRatio"];
  cmp.nearPlane = j["near_plane"];
  cmp.farPlane = j["far_plane"];
}
