#include "TextureManager.h"

#include <memory>

#include "TextureData.h"
#include "Logger.h"

std::shared_ptr<TextureData>
TextureManager::load(const char *path,
                     const TextureConfig &config) {
  auto it = m_Cache.find(path);
  if (it != m_Cache.end() && it->second.config == config) 
      return it->second.texture;

  // create texture
  TextureData tex;
  tex.setConfig(config);
  tex.loadFile(path);

  // cache texture
  cache_entry newEntry{std::make_shared<TextureData>(std::move(tex)), config};
  m_Cache[path] = newEntry;

  return m_Cache[path].texture;
}

std::shared_ptr<TextureData> TextureManager::cache(std::shared_ptr<TextureData> tex){
  // check cache for entry
  auto it = m_Cache.find(tex->filePath);
  if(it != m_Cache.end() && it->second.config == tex->config)
    return it->second.texture; // return existing
     
  // make new cache
  m_Cache[tex->filePath] = cache_entry{tex, tex->config};
  return tex;
}
