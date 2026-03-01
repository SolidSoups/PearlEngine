#pragma once

#include "json_common.h"

struct CapsuleColliderComponent {
  glm::vec3 a{0.0f, 1.0f, 0.0f};
  glm::vec3 b{0.0f, 2.0f, 0.0f};
  float radius{1.0f};
};

inline void to_json(json& j, const CapsuleColliderComponent& cmp){
  j["a"] = cmp.a;
  j["b"] = cmp.b;
  j["radius"] = cmp.radius;
}

inline void from_json(const json& j, CapsuleColliderComponent& cmp){
  cmp.a =   j["a"];
  cmp.b =   j["b"];
  cmp.radius =   j["radius"];
}
