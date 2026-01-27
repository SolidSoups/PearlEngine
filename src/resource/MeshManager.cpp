#include "MeshManager.h"

#include <filesystem>
#include <cstring>
#include <optional>
#include <fstream>
#include <map>

#include "Mesh.h"
#include "Logger.h"

#include "Mesh.h"

bool MeshManager::loadAndParseObjFile(
    const char *path, std::vector<glm::vec3> &outObjVertices,
    std::vector<glm::vec2> &outObjUvs, std::vector<glm::vec3> &outObjNormals,
    std::vector<unsigned int> &outObjIndices) {
  // clear output references
  outObjIndices.clear();
  outObjVertices.clear();
  outObjUvs.clear();
  outObjNormals.clear();

  std::ifstream file;
  file.exceptions(std::ifstream::badbit); // ensure exceptions

  try {
    LOG_INFO << "Attempting to open OBJ file: " << path;
    file.open(path);

    if (!file.is_open()) {
      LOG_ERROR << "Failed to open OBJ file: " << path;
      return false;
    }

    LOG_INFO << "Successfully opened OBJ file";

    // read line by line
    int faceIteration = 0;
    int lineCount = 0;
    std::string line;
    while (std::getline(file, line)) {
      lineCount++;
      char lineHeader[124];
      int result = sscanf(line.c_str(), "%s", lineHeader);

      // skip empty/whitespace-only lines
      if (result == EOF) {
        continue;
      }

      // otherwise, lets parse
      if (strcmp(lineHeader, "v") == 0) { // parse vertex
        glm::vec3 vertex;
        sscanf(line.c_str(), "v %f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
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
        sscanf(line.c_str(), "vn %f %f %f\n", &normal.x, &normal.y, &normal.z);
        outObjNormals.push_back(normal);
      }

      // parse the dreaded face
      else if (strcmp(lineHeader, "f") == 0) {
        faceIteration++;
        std::string vertex1, vertex2, vertex3;
        unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
        int matches = sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d\n",
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
    LOG_ERROR << "File stream failed to read '" << path << "', " << e.what();
    return false;
  } catch (std::exception &e){
    LOG_ERROR << "Unexpected error reading file '" << path << "', " << e.what();
  }
  return true;
}
void MeshManager::reformatObjToOpenGl(
    const std::vector<glm::vec3> &objVertices,
    const std::vector<glm::vec2> &objUvs,
    const std::vector<glm::vec3> &objNormals,
    const std::vector<unsigned int> &objIndices,
    std::vector<float> &outVertices, std::vector<unsigned int> &outIndices) {
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

std::shared_ptr<Mesh> MeshManager::loadOBJ(const char *filePath) {
  LOG_INFO << "Loading OBJ from filepath: " << filePath;
  auto it = m_Cache.find(filePath);
  if (it != m_Cache.end()) {
    // this mesh is cached, don't bother creating a new one
    return it->second;
  }

  // parse obj file
  std::vector<glm::vec3> objVertices;
  std::vector<glm::vec2> objUvs;
  std::vector<glm::vec3> objNormals;
  std::vector<unsigned int> objIndices;
  bool success = loadAndParseObjFile(filePath, objVertices, objUvs, objNormals,
                                     objIndices);
  if (!success) {
    LOG_ERROR << "Failed to load and parse Obj file: " << filePath;
    return nullptr;
  }
  LOG_INFO << "Loaded and parsed obj file, vertices: " << objVertices.size()
           << ", uvs: " << objUvs.size() << ", normals: " << objNormals.size()
           << ", indices: " << objIndices.size() << ";";

  // convert to opengl format
  std::vector<float> vertices;
  std::vector<unsigned int> indices;
  reformatObjToOpenGl(objVertices, objUvs, objNormals, objIndices, vertices,
                      indices);
  LOG_INFO << "Reformated loaded obj file to OpenGL format";

  m_Cache[filePath] = std::make_shared<Mesh>(vertices, indices);

  return m_Cache[filePath];
}
