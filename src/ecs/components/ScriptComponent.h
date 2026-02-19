#pragma once

#include <string>

#include <sol/sol.hpp>

class ScriptComponent {
public:
  sol::table scriptEnv;
  std::string scriptPath;
  bool loaded = false;
};
