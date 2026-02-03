#include "MeshManager.h"

#include <filesystem>
#include <cstring>
#include <optional>
#include <fstream>
#include <map>
#include <iostream>

#include "Mesh.h"
#include "Logger.h"

#include "Mesh.h"

std::vector<std::string> splitWhiteSpace(const char *line) {
  const char *ptr = line;
  std::vector<std::string> f_verts;
  std::string temp_str;
  ptr += 2; // skip whitespace

  while (*ptr != '\0') {
    if (*ptr == ' ') {
      if (!temp_str.empty()) { // skip pushing empty strings
        f_verts.push_back(temp_str);
        temp_str = "";
      }
    } else {
      temp_str += *ptr;
    }

    ptr++;
  }
  // push final vertex (if any)
  if (!temp_str.empty()) {
    f_verts.push_back(temp_str);
  }
  return f_verts;
}

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
    file.open(path);

    if (!file.is_open()) {
      LOG_ERROR << "Failed to open OBJ file: " << path;
      return false;
    }

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
        // uv.y = 1.0f - uv.y;
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
        std::vector<std::string> tokens = splitWhiteSpace(line.c_str());

        size_t num_vertices = tokens.size();
        if (num_vertices < 3) {
          LOG_ERROR << "Face must have at least 3 vertices";
          return false;
        }

        // construct sets of split up vertices
        // (so we can access them very easily without parsing)
        std::vector<std::array<unsigned int, 3>> face_vertices(num_vertices);
        for (int i = 0; i < tokens.size(); i++) {
          int matches = sscanf(tokens[i].c_str(), "%d/%d/%d",
                               &face_vertices[i][0], // pos
                               &face_vertices[i][1], // uv
                               &face_vertices[i][2]  // normal
          );
          if (matches != 3) {
            LOG_WARNING << "Unexpected number of matches (" << matches
                        << ") when parsing vertex through faces";
            return false;
          }
        }

        // construct triangles using Fan Triangulization method
        // (using first vertex as the center)
        for (size_t i = 0; i < num_vertices - 1; i++) {
          // out: v[0], v[i], v[i+1]
          outObjIndices.push_back(face_vertices[0][0]);
          outObjIndices.push_back(face_vertices[0][1]);
          outObjIndices.push_back(face_vertices[0][2]);

          outObjIndices.push_back(face_vertices[i][0]);
          outObjIndices.push_back(face_vertices[i][1]);
          outObjIndices.push_back(face_vertices[i][2]);

          outObjIndices.push_back(face_vertices[i + 1][0]);
          outObjIndices.push_back(face_vertices[i + 1][1]);
          outObjIndices.push_back(face_vertices[i + 1][2]);
        }
      }
    }
    file.close();
  } catch (std::ifstream::failure e) {
    LOG_ERROR << "File stream failed to read '" << path << "', " << e.what();
    return false;
  } catch (std::exception &e) {
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

  // convert to opengl format
  std::vector<float> vertices;
  std::vector<unsigned int> indices;
  reformatObjToOpenGl(objVertices, objUvs, objNormals, objIndices, vertices,
                      indices);

  m_Cache[filePath] = std::make_shared<Mesh>(vertices, indices);

  return m_Cache[filePath];
}
