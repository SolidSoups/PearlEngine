#pragma once

#include <glm/glm.hpp>

#include "AssetLoader.h"
#include "AssetsMacros.h"
#include "Mesh.h"

class OBJLoader : public AssetLoader<Mesh> {
  private:
    bool LoadAndParseObjFile(const char *path,
                      std::vector<unsigned int> &outObjIndices,
                      std::vector<glm::vec3> &outObjVertices,
                      std::vector<glm::vec2> &outObjUvs,
                      std::vector<glm::vec3> &outObjNormals);
    void ReformatObjToOpenGl(const std::vector<unsigned int> &objIndices,
                             const std::vector<glm::vec3> &objVertices,
                             const std::vector<glm::vec2> &objUvs,
                             const std::vector<glm::vec3> &objNormals,
                             std::vector<float> &outVertices,
                             std::vector<unsigned int> &outIndices);

  public:
    std::optional<Mesh> LoadAsset(const pe::File *file) override;
};

STATIC_REGISTER_ASSET_LOADER(".obj", OBJLoader, Mesh);
