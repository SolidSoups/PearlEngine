#pragma once


#include <glm/glm.hpp>

#include "IAssetConverter.h"

class OBJ_AssetConverter : public IAssetConverter{
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
    std::unique_ptr<IAsset> ConvertToAsset(const pe::FileDescriptor* file){

    }
};
