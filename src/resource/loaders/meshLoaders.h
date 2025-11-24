#pragma once


#include <vector>

#include "MeshData.h"
#include "vertex.h"
#include "Mesh.h"

MeshOldHandle CreateMesh(
  const std::vector<Vertex>& vertices, 
  const std::vector<unsigned int>& indices);
MeshOldHandle CreateMeshNew(
  const std::vector<float>& vertices, 
  const std::vector<unsigned int>& indices);

MeshHandle CreateMeshFromObjFile(const char* objPath); 
