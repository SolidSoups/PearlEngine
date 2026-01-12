#pragma once

struct MaterialData;
struct ShaderData;

#include <memory>
class MaterialLoader {
  
public:
  std::shared_ptr<MaterialData> create(std::shared_ptr<ShaderData> shader);
};
