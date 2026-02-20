#pragma once


#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

#include "TextureData.h"
#include "TextureConfig.h"

class TextureManager {
private:
  struct cache_entry{
    std::shared_ptr<TextureData> texture;
    TextureConfig config;
    bool operator==(const cache_entry& other) const{
      return texture == other.texture;
    }
  };
  std::unordered_map<std::string, cache_entry> m_Cache;

public:
  size_t getCacheSize() const { return m_Cache.size(); }
  size_t calcMemorySize() const {
    size_t totalSize = 0;
    for(auto& [key, entry] : m_Cache){
      if(entry.texture){
        totalSize += entry.texture->getMemorySize();
      }
    }
    return totalSize;
  }

public:
  std::shared_ptr<TextureData> load(const char* path, const TextureConfig& config = TextureConfig());
};
