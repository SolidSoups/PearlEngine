#include "ShaderLoader.h"

#include <vector>

#include "FileSystem.h"
#include "Logger.h"
#include "ShaderData.h"

std::shared_ptr<ShaderData> ShaderLoader::load(const char* vertPath, const char* fragPath){
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
  
  return std::make_shared<ShaderData>(vertBytes.data(), fragBytes.data());
}
