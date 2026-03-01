#pragma once

#include "json_common.h"

struct BoxColliderComponent {
  glm::vec3 center{0.0f};
  glm::vec3 size{1.0f};
};

inline void to_json(json& j, const BoxColliderComponent& cmp){
  j["center"] = cmp.center;
  j["size"] = cmp.size;
}

inline void from_json(const json& j, BoxColliderComponent& cmp){
  cmp.center = j["center"];
  cmp.size = j["size"];
}
