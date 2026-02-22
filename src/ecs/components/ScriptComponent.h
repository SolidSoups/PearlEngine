#pragma once
#include <string>

#include <sol/sol.hpp>

#include "json_common.h"

class ScriptComponent {
public:
  std::string scriptPath;
  sol::table scriptEnv;
  bool loaded = false;
  bool enabled = false;
  bool needsReload = false;
  bool hasError = false;
};

inline void to_json(json& j, const ScriptComponent& cmp) {
  j["script_path"] = cmp.scriptPath;
}

inline void from_json(const json& j, ScriptComponent& cmp) {
  cmp.scriptPath = j["script_path"];
}
