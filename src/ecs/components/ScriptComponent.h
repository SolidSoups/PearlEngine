#pragma once

#include <string>

#include <sol/sol.hpp>

#include "json_common.h"

class ScriptComponent {
public:
  sol::table scriptEnv;
  std::string scriptPath;
  bool loaded = false;
  bool needsReload = false;
  bool failed = false;
  std::string failure_reason;
};

inline void to_json(json& j, const ScriptComponent& cmp) {
  j["script_path"] = cmp.scriptPath;
}

inline void from_json(const json& j, ScriptComponent& cmp) {
  cmp.scriptPath = j["script_path"];
}
