#pragma once

#include <cfloat>
#include "json_common.h"

struct LevelTime {
  float level1Time = FLT_MAX;
  bool level1Active = false;
  float level2Time = FLT_MAX;
  bool level2Active = false;
  float level3Time = FLT_MAX;
  bool level3Active = false;
};

inline void to_json(json& j, const LevelTime& lvl){
  j["level1Time"] = lvl.level1Time;
  j["level2Time"] = lvl.level2Time;
  j["level3Time"] = lvl.level3Time;
}

inline void from_json(const json& j, LevelTime& lvl){
	lvl.level1Time =   j["level1Time"];
	lvl.level2Time =   j["level2Time"];
	lvl.level3Time =   j["level3Time"];
}

