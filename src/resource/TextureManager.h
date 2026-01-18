#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "TextureData.h"
#include "TextureConfig.h"

class TextureManager {
private:
  std::unordered_map<std::string, std::shared_ptr<TextureData>> m_Cache;

public:
  size_t getCacheSize() const { return m_Cache.size(); }
  size_t calcMemorySize() const {
    size_t totalSize = 0;
    for(auto& [key, shared_ptr] : m_Cache){
      if(shared_ptr.get()){
        totalSize += shared_ptr->getMemorySize();
      }
    }
    return totalSize;
  }

public:
  std::shared_ptr<TextureData> load(const char* path, const TextureConfig& config = TextureConfig());
};
