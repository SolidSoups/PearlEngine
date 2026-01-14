#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "ShaderData.h"

class ShaderManager {
private:
  std::unordered_map<std::string, std::shared_ptr<ShaderData>> m_Cache;

public:
  size_t getCacheSize() const { return m_Cache.size(); }
  size_t calcMemorySize() const {
    size_t totalSize = 0;
    for(const auto& [key, value] : m_Cache){
      if(value.get()){
        totalSize += value->getMemorySize();
      }
    }
    return totalSize;
  }

public:
  std::shared_ptr<ShaderData> load(const char* vertPath, const char* fragPath);
  void recompileAll();

};
