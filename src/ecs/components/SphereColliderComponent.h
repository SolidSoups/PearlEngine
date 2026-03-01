#pragma once

#include <glm/glm.hpp>

#include "json_common.h"

struct SphereColliderComponent {
  glm::vec3 position{0.0f};
  float radius{1.0f};
};

// serialization

inline void to_json(json& j, const SphereColliderComponent& cmp){
  j["position"] = cmp.position;
  j["radius"] = cmp.radius;
}

inline void from_json(const json& j, SphereColliderComponent& cmp){
  cmp.position = j["position"];
  cmp.radius = j["radius"];
}
