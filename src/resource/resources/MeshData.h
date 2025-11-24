#pragma once

#include <cstddef>
#include <vector>


#include "ResourceMacros.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "ElementBuffer.h"
#include "Handle.h"
#include "MaterialData.h"
#include <glad/glad.h>

struct MeshData {
    GLuint VAO = 0;
    GLuint VBO = 0;
    GLuint EBO = 0;
    size_t vertexCount;
    size_t indexCount;
};
PEARL_DEFINE_RESOURCE(MeshOld, MeshData)

void DestroyMesh(MeshOldHandle handle);
void RenderMesh(MeshOldHandle handle);
