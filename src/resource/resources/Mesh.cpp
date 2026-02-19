#include "Mesh.h"

#include <immintrin.h>
#include <cstring>

#include <glm/glm.hpp>

#include "Logger.h"


constexpr int IN_VERTEX_SIZE =  8;
constexpr int OUT_VERTEX_SIZE = 11;



// f0 =  [px, py, pz, uvx, uvy, nx, ny, nz, 0, 0, 0]
//        0   1   2   3    4    5   6   7   8  9  10 
glm::vec3 compute_tangent(const float* f0, const float* f1, const float* f2){
  // edges
  glm::vec3 e1(f1[0] - f0[0], f1[1] - f0[1], f1[2] - f0[2]);
  glm::vec3 e2(f2[0] - f0[0], f2[1] - f0[1], f2[2] - f0[2]);

  // uv deltas
  float du1 = f1[3] - f0[3];
  float dv1 = f1[4] - f0[4];
  float du2 = f2[3] - f0[3];
  float dv2 = f2[4] - f0[4];
  
  // inverse determinant (2d cross product)
  float det = du1 * dv2 - du2 * dv1;
  if(std::abs(det) < 1e-6f)
    return glm::vec3(1.0f, 0.0f, 0.0f);
  float f = 1.0f / det;

  glm::vec3 tangent;
  tangent.x = f * (dv2 * e1.x - dv1 * e2.x);
  tangent.y = f * (dv2 * e1.y - dv1 * e2.y);
  tangent.z = f * (dv2 * e1.z - dv1 * e2.z);

  return glm::normalize(tangent);
}


void generateTangents(const std::vector<float> &vertices, const std::vector<unsigned int> &indices, std::vector<float> &out_new_vertices){
  // TANGENT PASS 1: expand to 11 floats per vertex, add 3 extra 0.0f at end
  out_new_vertices.resize(vertices.size() / IN_VERTEX_SIZE * OUT_VERTEX_SIZE, 0.0f);

    // get the memory locations of the buffers
  const float *vertices_ptr = vertices.data();
  float *new_vertices_ptr = out_new_vertices.data();

    // copy vertices array to new_vertices array, ignoring the tangent positions
  size_t verticesSize = vertices.size();
  for (size_t i = 0; i < verticesSize; i += IN_VERTEX_SIZE) {
    size_t dst = (i / IN_VERTEX_SIZE) * OUT_VERTEX_SIZE;

#ifdef PEARL_HAS_AVX2
    _mm256_storeu_ps(new_vertices_ptr + dst, _mm256_loadu_ps(vertices_ptr + i));
#else
    std::memcpy(new_vertices_ptr + dst, vertices_ptr + i, 8 * sizeof(float));
#endif
  }

  // TANGENT PASS 2: iterate through faces, accumulate tangent
  size_t index_count = indices.size();
  for(size_t i=0; i<index_count; i+=3){
    size_t i0 = indices[i + 0];
    size_t i1 = indices[i + 1];
    size_t i2 = indices[i + 2];
   
    float* f0 = new_vertices_ptr + i0 * OUT_VERTEX_SIZE;
    float* f1 = new_vertices_ptr + i1 * OUT_VERTEX_SIZE;
    float* f2 = new_vertices_ptr + i2 * OUT_VERTEX_SIZE;

    glm::vec3 tangent = compute_tangent(f0, f1, f2);

    // accumulate tangent into slots
    f0[8] += tangent.x; f0[9] += tangent.y; f0[10] += tangent.z;
    f1[8] += tangent.x; f1[9] += tangent.y; f1[10] += tangent.z;
    f2[8] += tangent.x; f2[9] += tangent.y; f2[10] += tangent.z;
  }
  
  // TANGENT PASS 3: normalize all tangents
  size_t vertex_count = out_new_vertices.size() / OUT_VERTEX_SIZE;
  for(size_t i=0; i<vertex_count; i++){
    float* v = new_vertices_ptr + i * OUT_VERTEX_SIZE;
    glm::vec3 t = *reinterpret_cast<glm::vec3*>(v + 8); // reinterpret tangent indices as vec3
    t = glm::normalize(t);
    v[8] = t.x; v[9] = t.y; v[10] = t.z;
  }
}

Mesh::Mesh(const std::vector<float> &vertices,
           const std::vector<unsigned int> &indices, const char *filepath)
    : mFilePath(filepath) {
  // always generate tangents
  std::vector<float> new_vertices;
  generateTangents(vertices, indices, new_vertices);

  // generate buffers
  m_VAO.GenerateVertexArrays();
  m_VAO.Bind();

  // upload 11 floats per vertex
  m_VBO.GenerateVertexBuffers(new_vertices.data(), new_vertices.size() / OUT_VERTEX_SIZE,
                              OUT_VERTEX_SIZE);

  // upload triangle indices
  m_EBO.GenerateElementBuffers(indices.data(), indices.size());

  // attribute 0: position (3 floats start at 0)
  m_VAO.DefineAttribute(m_VBO, 0, 3, GL_FLOAT, GL_FALSE,
                        OUT_VERTEX_SIZE * sizeof(float), (void *)0);
  // attribute 1: UV (2 floats offset at 3)
  m_VAO.DefineAttribute(m_VBO, 1, 2, GL_FLOAT, GL_FALSE,
                        OUT_VERTEX_SIZE * sizeof(float),
                        (void *)(3 * sizeof(float)));
  // attribute 2: Normal (3 floats offset at 5)
  m_VAO.DefineAttribute(m_VBO, 2, 3, GL_FLOAT, GL_FALSE,
                        OUT_VERTEX_SIZE * sizeof(float),
                        (void *)(5 * sizeof(float)));

  // attribute 3: Tangent (3 floats offset at 8)
  m_VAO.DefineAttribute(m_VBO, 3, 3, GL_FLOAT, GL_FALSE,
                        OUT_VERTEX_SIZE * sizeof(float),
                        (void *)(8 * sizeof(float)));

  m_MemorySize =
      new_vertices.size() * sizeof(float) + indices.size() * sizeof(unsigned int);
}
Mesh::~Mesh() {}

void Mesh::Draw() {
  m_VAO.Bind();
  glDrawElements(GL_TRIANGLES, m_EBO.GetElementCount(), GL_UNSIGNED_INT, 0);
}
