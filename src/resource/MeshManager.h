#pragma once

#include <unordered_map>
#include <memory>
#include <string>

#include <glm/glm.hpp>

#include "Mesh.h"

class Mesh;

class MeshManager {
private:
  std::unordered_map<std::string, std::shared_ptr<Mesh>> m_Cache;

public:
  std::shared_ptr<Mesh> loadOBJ(const char *filePath);

  const size_t getCacheSize() const { return m_Cache.size(); }
  const size_t calcMemorySize() const {
    size_t totalSize = 0;
    for(const auto& [str, shared_ptr] : m_Cache){
      if(shared_ptr.get()){
        totalSize += shared_ptr->getMemorySize();
      }
    }
    return totalSize;
  }

private:
  bool loadAndParseObjFile(const char *path,
                           std::vector<glm::vec3> &outObjVertices,
                           std::vector<glm::vec2> &outObjUvs,
                           std::vector<glm::vec3> &outObjNormals,
                           std::vector<unsigned int> &outObjIndices);
  void reformatObjToOpenGl(const std::vector<glm::vec3> &objVertices,
                           const std::vector<glm::vec2> &objUvs,
                           const std::vector<glm::vec3> &objNormals,
                           const std::vector<unsigned int> &objIndices,
                           std::vector<float> &outVertices,
                           std::vector<unsigned int> &outIndices);
};
