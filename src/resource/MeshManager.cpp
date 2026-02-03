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
        for (size_t i = 1; i < num_vertices - 1; i++) {
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

glm::vec3 computeTangent(
  const glm::vec3 &p0, const glm::vec3 &p1, const glm::vec3 &p2,
  const glm::vec2 &uv0, const glm::vec2 &uv1, const glm::vec2 &uv2
){
  // Edge vectors (in 3d space) 
  glm::vec3 e1 = p1 - p0;
  glm::vec3 e2 = p2 - p0;

  // uv deltas
  float du1 = uv1.x - uv0.x;
  float dv1 = uv1.y - uv0.y;
  float du2 = uv2.x - uv0.x;
  float dv2 = uv2.y - uv0.y;

  // inverse determinant
  float det = du1 * dv2 - du2 * dv1;
  if(std::abs(det) < 1e-6f){
    // degenerates
    return glm::vec3(1.0f, 0.0f, 0.0);
  }
  float f = 1.0 / det;

  // calc tangent
  glm::vec3 tangent;
  tangent.x = f * (dv2 * e1.x - dv1 * e2.x);
  tangent.y = f * (dv2 * e1.y - dv1 * e2.y);
  tangent.z = f * (dv2 * e1.z - dv1 * e2.z);

  return glm::normalize(tangent);
}


void MeshManager::reformatObjToOpenGl(
    const std::vector<glm::vec3> &objVertices,
    const std::vector<glm::vec2> &objUvs,
    const std::vector<glm::vec3> &objNormals,
    const std::vector<unsigned int> &objIndices,
    std::vector<float> &outVertices, std::vector<unsigned int> &outIndices) {
  outVertices.clear();
  outIndices.clear();

  std::map<std::array<unsigned int, 3>, unsigned int> faceToIndex;
  unsigned int nextIndex = 0;

  // Process triangles (every 9 indices = 3 vertices * 3 components each)
  for (size_t i = 0; i < objIndices.size(); i += 9) {
    // generate the index for the vertex
    std::array<unsigned int, 3> key = {objIndices[i], objIndices[i + 1],
                                       objIndices[i + 2]};
    glm::vec3 pos[3];
    glm::vec2 uv[3];
    glm::vec3 norm[3];

    for(int v = 0; v < 3; v++){
      size_t base = i + v * 3;
      pos[v] = objVertices[objIndices[base + 0] - 1];
      uv[v] = objUvs[objIndices[base + 1] - 1];
      norm[v] = objNormals[objIndices[base + 2] - 1];
    }

    // compute tangent for this triangle
    glm::vec3 tangent = computeTangent(pos[0], pos[1], pos[2],
                                       uv[0], uv[1], uv[2]);

    // add each vertex
    for(int v = 0; v < 3; v++){
      std::array<unsigned int, 3> key = {
        objIndices[i + v*3 + 0],
        objIndices[i + v*3 + 1],
        objIndices[i + v*3 + 2]
      };

      if(faceToIndex.find(key) == faceToIndex.end()){
        faceToIndex[key] = nextIndex++;

        // position (3)
        outVertices.push_back(pos[v].x);
        outVertices.push_back(pos[v].y);
        outVertices.push_back(pos[v].z);
        // uv (2)
        outVertices.push_back(uv[v].x);
        outVertices.push_back(uv[v].y);
        // normal (3)
        outVertices.push_back(norm[v].x);
        outVertices.push_back(norm[v].y);
        outVertices.push_back(norm[v].z);
        // tangent (3)
        outVertices.push_back(tangent.x);
        outVertices.push_back(tangent.y);
        outVertices.push_back(tangent.z);
      }
      outIndices.push_back(faceToIndex[key]);
    }
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
