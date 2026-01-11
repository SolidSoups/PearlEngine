#include "ShaderLoader.h"

#include <vector>

#include "FileSystem.h"
#include "Logger.h"

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
  
  return std::make_shared<ShaderData>(vertBytes.data(), fragBytes.data());
}
