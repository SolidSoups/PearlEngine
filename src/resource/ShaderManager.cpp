#include "ShaderManager.h"

#include <vector>

#include "FileSystem.h"
#include "Logger.h"
#include "ShaderData.h"

std::shared_ptr<ShaderData> ShaderManager::load(const char* vertPath, const char* fragPath){
  auto it = m_Cache.find(std::string(vertPath) + fragPath);
  if(it != m_Cache.end()){
    return it->second;
  }
  
  m_Cache[std::string(vertPath) + fragPath] = std::make_shared<ShaderData>(vertPath, fragPath);
  return m_Cache[std::string(vertPath) + fragPath]; 
}

void ShaderManager::recompileAll(){
  for(auto& [key, shader] : m_Cache){
    shader->reload();
  }
}
