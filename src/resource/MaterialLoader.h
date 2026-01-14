#pragma once

struct Material;
struct ShaderData;

#include <memory>
class MaterialLoader {
  
public:
  static std::shared_ptr<Material> create(std::shared_ptr<ShaderData> shader);
};
