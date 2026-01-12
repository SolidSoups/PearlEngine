#pragma once

#include <memory>
#include <vector>
#include <glm/glm.hpp>

class Mesh;

class MeshLoader {
private:
  static bool loadAndParseObjFile(const char *path,
                                  std::vector<glm::vec3> &outObjVertices,
                                  std::vector<glm::vec2> &outObjUvs,
                                  std::vector<glm::vec3> &outObjNormals,
                                  std::vector<unsigned int> &outObjIndices);

  static void reformatObjToOpenGl(const std::vector<glm::vec3> &objVertices,
                                  const std::vector<glm::vec2> &objUvs,
                                  const std::vector<glm::vec3> &objNormals,
                                  const std::vector<unsigned int> &objIndices,
                                  std::vector<float> &outVertices,
                                  std::vector<unsigned int> &outIndices);

public:
  static std::shared_ptr<Mesh> loadObj(const char *filePath);
};
