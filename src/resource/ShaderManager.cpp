#include "ShaderManager.h"

#include <vector>

#include "FileSystem.h"
#include "Logger.h"
#include "ShaderData.h"

std::shared_ptr<ShaderData> ShaderManager::load(const char* vertPath, const char* fragPath){
  auto it = m_Cache.find(std::string(vertPath) + fragPath);
  if(it != m_Cache.end()){
    // is this cache valid? otherwise create a new one
    if(auto lock = it->second.lock()){
      return lock;
    }
  }
  auto newShader = std::make_shared<ShaderData>(vertPath, fragPath);
  m_Cache[std::string(vertPath) + fragPath] = newShader;
  return newShader;
}

void ShaderManager::recompileAll(){
  for(auto& [key, weak_shader] : m_Cache){
    if(auto shader = weak_shader.lock())
      shader->reload();
  }
}
