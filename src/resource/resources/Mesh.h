#pragma once

#include <vector>

#include "ElementBuffer.h"
#include "IResource.h"
#include "Mesh_Asset.h"
#include "ResourceMacros.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

class Mesh : public IResource {
    RESOURCE_CLASS(Mesh);

  private:
    VertexArray m_VAO;
    VertexBuffer m_VBO;
    ElementBuffer m_EBO;

  public:
    Mesh(const std::vector<float> &vertices,
         const std::vector<unsigned int> &indices);
    Mesh(const Mesh &) = delete; // delete copy constructor
    Mesh &operator=(const Mesh &) = delete;

    Mesh(Mesh &&) noexcept = default;
    Mesh &operator=(Mesh &&) noexcept = default;

    ~Mesh();
    void Draw();
};
PEARL_DEFINE_RESOURCE(Mesh, Mesh, Mesh_Asset);
