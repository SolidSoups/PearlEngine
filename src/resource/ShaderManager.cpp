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

  // load shader files
  std::vector<char> vertBytes, fragBytes;
  if(!FileSystem::loadFile(vertPath, vertBytes)){
    LOG_ERROR << "Failed to load vertex file at path: " << vertPath;
    return nullptr;
  }
  if(!FileSystem::loadFile(fragPath, fragBytes)){
    LOG_ERROR << "Failed to load frag file at path: " << fragPath;
    return nullptr;
  }
  vertBytes.push_back('\0');
  fragBytes.push_back('\0');
  
  m_Cache[std::string(vertPath) + fragPath] = std::make_shared<ShaderData>(vertBytes.data(), fragBytes.data());
  return m_Cache[std::string(vertPath) + fragPath]; 
}
