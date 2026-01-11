#pragma once

#include <memory>

struct ShaderData;

class ShaderLoader {
public:
  static std::shared_ptr<ShaderData> load(const char* vertPath, const char* fragPath);
};
