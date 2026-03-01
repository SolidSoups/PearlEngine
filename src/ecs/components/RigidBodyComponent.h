#pragma once

#include "json_common.h"

struct RigidBodyComponent {
  float mass{1.0f};
  float gravity{9.87f};

  // runtime
  glm::vec3 velocity{0.0f};
};

inline void to_json(json& j, const RigidBodyComponent& cmp){
  j["mass"] = cmp.mass;
  j["gravity"] = cmp.gravity;
}

inline void from_json(const json& j, RigidBodyComponent& cmp){
  cmp.mass = j["mass"];
  cmp.gravity = j["gravity"];
}

