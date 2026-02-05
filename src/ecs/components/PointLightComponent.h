#pragma once

#include "json_common.h"
#include "PointLightData.h"

struct PointLightComponent {
  PointLightData data;

  PointLightComponent() = default;
};

inline void to_json(json &j, const PointLightComponent &cmp) {
  j["color"] = cmp.data.color;
  j["intensity"] = cmp.data.intensity;
  j["radius"] = cmp.data.radius;
  j["linear"] = cmp.data.linearAttenuation;
  j["quadratic"] = cmp.data.quadraticAttenuation;
}

inline void from_json(const json &j, PointLightComponent &cmp) {
	cmp.data.color = j["color"];
	cmp.data.intensity = j["intensity"];
	cmp.data.radius = j["radius"];
	cmp.data.linearAttenuation = j["linear"];
	cmp.data.quadraticAttenuation = j["quadratic"];
}
