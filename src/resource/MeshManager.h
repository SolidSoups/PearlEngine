#pragma once

#include <unordered_map>
#include <memory>
#include <string>

#include <glm/glm.hpp>

class Mesh;

class MeshManager {

public:
  // loading functions for different filetypes
  std::shared_ptr<Mesh> loadOBJ(const char *filePath);

private:
  // helper functions
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

private:
  // the actual caching here
  std::unordered_map<std::string, std::shared_ptr<Mesh>> m_Cache;
};
