#pragma once

struct MaterialData;
struct ShaderData;

#include <memory>
class MaterialLoader {
  
public:
  static std::shared_ptr<MaterialData> create(std::shared_ptr<ShaderData> shader);
};
