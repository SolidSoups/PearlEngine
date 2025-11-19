#pragma once


#include <vector>

#include "MeshData.h"
#include "vertex.h"

MeshHandle CreateMesh(
  const std::vector<Vertex>& vertices, 
  const std::vector<unsigned int>& indices);
MeshHandle CreateMeshNew(
  const std::vector<Vertex>& vertices, 
  const std::vector<unsigned int>& indices);

MeshHandle CreateMeshFromObjFile(const char* objPath); 
