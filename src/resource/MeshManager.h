#pragma once

#include <unordered_map>
#include <memory>
#include <string>

#include <glm/glm.hpp>

#include "Mesh.h"
#include "Logger.h"

class Mesh;

class MeshManager {
private:
  std::unordered_map<std::string, std::weak_ptr<Mesh>> m_Cache;
  std::vector<std::weak_ptr<Mesh>> m_Tracks;

public:
  void track(std::shared_ptr<Mesh> meshPtr);
  std::shared_ptr<Mesh> loadOBJ(const char *filePath);

  const size_t getCacheSize() const {
    int count = 0;
    for (auto &[key, weak] : m_Cache) {
      if (weak.lock())
        count++;
    }
    for(auto& weak : m_Tracks){
      if(weak.lock())
        count++;
    }
    return count;
  }
  const size_t calcMemorySize() const {
    size_t totalSize = 0;
    for (const auto &[str, weak_ptr] : m_Cache) {
      if (auto lock = weak_ptr.lock()) {
        totalSize += lock->getMemorySize();
      }
    }
    for(const auto& weak_ptr : m_Tracks){
      if(auto lock = weak_ptr.lock())
        totalSize += lock->getMemorySize();
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
