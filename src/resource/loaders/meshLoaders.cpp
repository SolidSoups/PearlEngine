#include "meshLoaders.h"

#include <array>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <iostream>
#include <map>

#include "Logger.h"
#include "glad/glad.h"

#include "ResourceSystem.h"
#include "vertex.h"

namespace {
// TODO: should use vertex later on
bool ParseObjFile(const char* path,
                  std::vector<unsigned int>& outObjIndices, 
                  std::vector<glm::vec3>& outObjVertices,
                  std::vector<glm::vec2>& outObjUvs,
                  std::vector<glm::vec3>& outObjNormals){
  // clear output references
  outObjIndices.clear();
  outObjVertices.clear();
  outObjUvs.clear();
  outObjNormals.clear();

  std::ifstream file;
  file.exceptions(std::ifstream::badbit); // ensure exceptions

  try{
    file.open(path);
    
    // read line by line
    int faceIteration = 0;
    std::string line;
    while(std::getline(file, line)){
      char lineHeader[124];
      int result = sscanf(line.c_str(), "%s", lineHeader);

      // we've reached the end of file, lets quit out
      if(result == EOF)
        break;

      // otherwise, lets parse
      if(strcmp(lineHeader, "v") == 0){               // parse vertex
        glm::vec3 vertex;
        sscanf(line.c_str(), "v %f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
        outObjVertices.push_back(vertex);
      } 

      // parse uv
      else if(strcmp(lineHeader, "vt") == 0){       
        glm::vec2 uv;
        sscanf(line.c_str(), "vt %f %f\n", &uv.x, &uv.y);
        outObjUvs.push_back(uv);
      } 

      // parse normal
      else if(strcmp(lineHeader, "vn") == 0){       
        glm::vec3 normal;
        sscanf(line.c_str(), "vn %f %f %f\n", &normal.x, &normal.y, &normal.z);
        outObjNormals.push_back(normal);
      } 

      // parse the dreaded face
      else if(strcmp(lineHeader, "f") == 0){        
        faceIteration++;
        std::string vertex1, vertex2, vertex3;
        unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
        int matches = sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d\n",
                            &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                            &vertexIndex[1], &uvIndex[1], &normalIndex[1],
                            &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
        if(matches != 9){
          LOG_ERROR << "Unexpected number of matches when parsing faces. Iteration: " << faceIteration;
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
  }
  catch(std::ifstream::failure e){
    LOG_ERROR << "File stream failed to read '" << path << "', " << e.what();
    return false;
  }
  return true;
};

/**
* Reformats the parsed obj format to a format suitable for OpenGl. Vertices consist of [Vertex, UV, Normal] and a list of indices control faces using vertex indices.
*/
bool ReformatObjToOpenGl(const std::vector<unsigned int>& objIndices,
                         const std::vector<glm::vec3>& objVertices,
                         const std::vector<glm::vec2>& objUvs,
                         const std::vector<glm::vec3>& objNormals,
                         std::vector<VertexNew>& outVertices,
                         std::vector<unsigned int>& outIndices){
  // clear the output refernces
  outVertices.clear();
  outIndices.clear();

  // we will keep track of the faces that have already been added
  std::map<std::array<unsigned int, 3>, unsigned int> faceToIndex;
  unsigned int nextIndex = 0;

  // iterate over every 3 elements of a face vertex
  for(size_t i = 0; i < objIndices.size(); i += 3){
    // generate the index for the vertex
    std::array<unsigned int, 3> key = {objIndices[i], objIndices[i+1], objIndices[i+2]};

    // generate a new index if new, otherwise reuse existing
    if(faceToIndex.find(key) == faceToIndex.end()){
      faceToIndex[key] = nextIndex++;

      // also create a vertex since this is a new index
      // (also, remind yourself that OBJ uses 1-based indexing!!!!)
      outVertices.push_back({
        objVertices[objIndices[i] - 1],
        objUvs[objIndices[i+1] - 1],
        objNormals[objIndices[i+2] - 1]
      });
    }
    outIndices.push_back(faceToIndex[key]);
  }

  return true;
}

bool LoadObjFile(const char* path, std::vector<VertexNew>& outVertices, std::vector<unsigned int>& outIndices){
  // load vertices and indices
  std::vector< unsigned int > temp_indices; // indices loaded here are constructed as such for one face; [v1, uv1, n1, v2, uv2, n2, v3, uv3, n3]
  std::vector< glm::vec3 > temp_vertices;
  std::vector< glm::vec2 > temp_uvs;
  std::vector< glm::vec3 > temp_normals;
  if(!ParseObjFile(path, temp_indices, temp_vertices, temp_uvs, temp_normals))
    return false;

  // generate correct vertices and indices
  // vertices (for every vertex) = [vertex.x, vertex.y, vertex.z, uv.x, uv.y, normal.x, normal.y, normal.z]
  // indices (for every face) = [vertexIndex1, vertexIndex2, vertexIndex3]
  
  // loop over every face...
  outVertices.clear();
  outIndices.clear();
  std::map<std::array<unsigned int, 3>, unsigned int> elementToId;
  unsigned int nextIndex = 0;
  for(size_t i=0; i<temp_indices.size(); i+=3){
    // generate the index for the vertex
    std::array<unsigned int, 3> key = {temp_indices[i], temp_indices[i+1], temp_indices[i+2]};

    // generate a new index if new, otherwise reuse existing
    if(elementToId.find(key) == elementToId.end()){
      elementToId[key] = nextIndex++;

      // also create a vertex since this is a new index
      // (also, remind yourself that OBJ uses 1-based indexing!!!!)
      outVertices.push_back({
        temp_vertices[temp_indices[i] - 1],
        temp_uvs[temp_indices[i+1] - 1],
        temp_normals[temp_indices[i+2] - 1]
      });
    }
    outIndices.push_back(elementToId[key]);
  }

  return true;
};
} // namespace

MeshHandle CreateMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices){
  GLuint VAO = 0;
  GLuint VBO = 0;
  GLuint EBO = 0;

  glGenVertexArrays(1, &VAO); // generate vertex array object
  glBindVertexArray(VAO); // bind the VAO in openGL's state machine

  glGenBuffers(1, &VBO); // generate buffers for the vertex buffer object
  glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind the VBO to the target GL_ARRAY_BUFFER
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * 8 * sizeof(float), vertices.data(), GL_STATIC_DRAW); // feed the target of GL_ARRAY_BUFFER (which is our VBO) the data for the vertices

  // only generate Element buffer object if we need it...
  if(indices.size() > 0){ 
    glGenBuffers(1, &EBO); // generate a buffer for EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // bind the buffer to target GL_ELEMENT_ARRAY_BUFFER
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW); // feed the buffer some data (mmmmmmmm)
  }

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // uv attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // reset bindings
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // create mesh data and get handle
  MeshHandle meshHandle = ResourceSystem::Get().Meshes().Create(
    { VAO, VBO, EBO, vertices.size(), indices.size() }
  );

  LOG_INFO << "Created mesh (id=" << meshHandle.id << ")";

  return meshHandle;
};

MeshHandle CreateMeshNew(const std::vector<VertexNew>& vertices, const std::vector<unsigned int>& indices){
  GLuint VAO = 0;
  GLuint VBO = 0;
  GLuint EBO = 0;

  glGenVertexArrays(1, &VAO); // generate vertex array object
  glBindVertexArray(VAO); // bind the VAO in openGL's state machine

  glGenBuffers(1, &VBO); // generate buffers for the vertex buffer object
  glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind the VBO to the target GL_ARRAY_BUFFER
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * 8 * sizeof(float), vertices.data(), GL_STATIC_DRAW); // feed the target of GL_ARRAY_BUFFER (which is our VBO) the data for the vertices

  // only generate Element buffer object if we need it...
  if(indices.size() > 0){ 
    glGenBuffers(1, &EBO); // generate a buffer for EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // bind the buffer to target GL_ELEMENT_ARRAY_BUFFER
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW); // feed the buffer some data (mmmmmmmm)
  }

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  // uv attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // color attribute TODO: normal attribute
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // reset bindings
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  // create mesh data and get handle
  MeshHandle meshHandle = ResourceSystem::Get().Meshes().Create(
    { VAO, VBO, EBO, vertices.size(), indices.size() }
  );

  LOG_INFO << "Created mesh (id=" << meshHandle.id << ")";

  return meshHandle;
};

MeshHandle CreateMeshFromObjFile(const char *path){
  std::vector<VertexNew> vertices;
  std::vector<unsigned int> indices;
  if(LoadObjFile(path, vertices, indices)){
    LOG_INFO << "Succeded in loading mesh, \n\tvertices: " << vertices.size() << "\n\tindices: " << indices.size();
    // TODO: implement a new shader for this new format
    MeshHandle newHandle = CreateMeshNew(vertices, indices);
    return newHandle;
  }

  return MeshHandle::Invalid();
}
