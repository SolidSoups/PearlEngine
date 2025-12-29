#include "OBJ_AssetConverter.h"

#include <cstring>
#include <fstream>
#include <map>
#include <optional>
#include <sstream>

#include "Mesh_Asset.h"

#include "Logger.h"

using std::nullopt;

bool OBJ_AssetConverter::LoadAndParseObjFile(
    const char *path, std::vector<unsigned int> &outObjIndices,
    std::vector<glm::vec3> &outObjVertices, std::vector<glm::vec2> &outObjUvs,
    std::vector<glm::vec3> &outObjNormals) {
    // clear output references
    outObjIndices.clear();
    outObjVertices.clear();
    outObjUvs.clear();
    outObjNormals.clear();

    std::ifstream file;
    file.exceptions(std::ifstream::badbit); // ensure exceptions

    try {
        file.open(path);

        // read line by line
        int faceIteration = 0;
        std::string line;
        while (std::getline(file, line)) {
            char lineHeader[124];
            int result = sscanf(line.c_str(), "%s", lineHeader);

            // we've reached the end of file, lets quit out
            if (result == EOF)
                break;

            // otherwise, lets parse
            if (strcmp(lineHeader, "v") == 0) { // parse vertex
                glm::vec3 vertex;
                sscanf(line.c_str(), "v %f %f %f\n", &vertex.x, &vertex.y,
                       &vertex.z);
                outObjVertices.push_back(vertex);
            }

            // parse uv
            else if (strcmp(lineHeader, "vt") == 0) {
                glm::vec2 uv;
                sscanf(line.c_str(), "vt %f %f\n", &uv.x, &uv.y);
                outObjUvs.push_back(uv);
            }

            // parse normal
            else if (strcmp(lineHeader, "vn") == 0) {
                glm::vec3 normal;
                sscanf(line.c_str(), "vn %f %f %f\n", &normal.x, &normal.y,
                       &normal.z);
                outObjNormals.push_back(normal);
            }

            // parse the dreaded face
            else if (strcmp(lineHeader, "f") == 0) {
                faceIteration++;
                std::string vertex1, vertex2, vertex3;
                unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
                int matches =
                    sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d\n",
                           &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                           &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                           &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
                if (matches != 9) {
                    LOG_ERROR << "Unexpected number of matches when parsing "
                                 "faces. Iteration: "
                              << faceIteration;
                    return false;
                }
                // vertex 1
                outObjIndices.push_back(vertexIndex[0]);
                outObjIndices.push_back(uvIndex[0]);
                outObjIndices.push_back(normalIndex[0]);
                // vertex 2
                outObjIndices.push_back(vertexIndex[1]);
                outObjIndices.push_back(uvIndex[1]);
                outObjIndices.push_back(normalIndex[1]);
                // vertex 3
                outObjIndices.push_back(vertexIndex[2]);
                outObjIndices.push_back(uvIndex[2]);
                outObjIndices.push_back(normalIndex[2]);
            }
        }
        file.close();
    } catch (std::ifstream::failure e) {
        LOG_ERROR << "File stream failed to read '" << path << "', "
                  << e.what();
        return false;
    }
    return true;
}
void OBJ_AssetConverter::ReformatObjToOpenGl(
    const std::vector<unsigned int> &objIndices,
    const std::vector<glm::vec3> &objVertices,
    const std::vector<glm::vec2> &objUvs,
    const std::vector<glm::vec3> &objNormals, std::vector<float> &outVertices,
    std::vector<unsigned int> &outIndices) {
    // clear the output refernces
    outVertices.clear();
    outIndices.clear();

    // we will keep track of the faces that have already been added
    std::map<std::array<unsigned int, 3>, unsigned int> faceToIndex;
    unsigned int nextIndex = 0;

    // iterate over every 3 elements of a face vertex
    for (size_t i = 0; i < objIndices.size(); i += 3) {
        // generate the index for the vertex
        std::array<unsigned int, 3> key = {objIndices[i], objIndices[i + 1],
                                           objIndices[i + 2]};

        // generate a new index if new, otherwise reuse existing
        if (faceToIndex.find(key) == faceToIndex.end()) {
            faceToIndex[key] = nextIndex++;

            const glm::vec3 &vertex = objVertices[objIndices[i] - 1];
            const glm::vec2 &uv = objUvs[objIndices[i + 1] - 1];
            const glm::vec3 &normal = objNormals[objIndices[i + 2] - 1];

            // also create a vertex since this is a new index
            // (also, remind yourself that OBJ uses 1-based indexing!!!!)
            outVertices.push_back(vertex.x);
            outVertices.push_back(vertex.y);
            outVertices.push_back(vertex.z);
            outVertices.push_back(uv.x);
            outVertices.push_back(uv.y);
            outVertices.push_back(normal.x);
            outVertices.push_back(normal.y);
            outVertices.push_back(normal.z);
        }
        outIndices.push_back(faceToIndex[key]);
    }
}
std::unique_ptr<IAsset>
OBJ_AssetConverter::ConvertToAsset(const pe::FileDescriptor *file) {
    // load vertices and indices
    std::vector<unsigned int>
        temp_indices; // indices loaded here are constructed as such for one
                      // face; [v1, uv1, n1, v2, uv2, n2, v3, uv3, n3]
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;
    if (!LoadAndParseObjFile(file->localPath.c_str(), temp_indices, temp_vertices, temp_uvs,
                      temp_normals)) {
        return nullptr;
    }

    // reformat the vertices, uvs, normals, indices to our standard
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    ReformatObjToOpenGl(temp_indices, temp_vertices, temp_uvs, temp_normals,
                        vertices, indices);

    LOG_INFO << "Succeded in loading mesh, \n\tvertices: "
        << vertices.size() << "\n\tindices: " << indices.size();

    // create a new asset
    auto asset = std::make_unique<Mesh_Asset>(vertices, indices);
    return asset;
}

std::unique_ptr<IAsset> OBJ_AssetConverter::CreateEmptyAsset(){
    auto asset = std::make_unique<Mesh_Asset>();
    return asset;
}
