#include "Geometry.h"

#include <glm/glm.hpp>

// Plane
const float Plane::vertices[] = {
    // position           // uv         // normal
    -0.5f, 0.0f, 0.5f, 0.0f, 0.0f,  0.0f,  1.0f, 0.0f,  0.5f, 0.0f, 0.5f,
    1.0f,  0.0f, 0.0f, 1.0f, 0.0f,  -0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f,
    1.0f,  0.0f, 0.5f, 0.0f, -0.5f, 1.0f,  1.0f, 0.0f,  1.0f, 0.0f};
const unsigned int Plane::indices[] = {0, 1, 2, 2, 1, 3};

void Sphere::generate_uvsphere(std::vector<float> &out_vertices,
                               std::vector<unsigned int> &out_indices,
                               int n_slices, int n_stacks) {
  // top pole
  out_vertices.push_back(0.0f);
  out_vertices.push_back(1.0f);
  out_vertices.push_back(0.0f);
  out_vertices.push_back(0);
  out_vertices.push_back(1);
  out_vertices.push_back(0.0f);
  out_vertices.push_back(1.0f);
  out_vertices.push_back(0.0f);


  // generate all the points we need
  for (size_t i = 0; i < n_stacks - 1; i++) {
    float phi = M_PI * double(i + 1) / double(n_stacks);
    for (int j = 0; j < n_slices; j++) {
      // calculate point
      float theta = 2 * M_PI * double(j) / double(n_slices);
      float x = glm::sin(phi) * std::cos(theta);
      float y = glm::cos(phi);
      float z = glm::sin(phi) * std::sin(theta);
      glm::vec3 next_point(x, y, z);

      out_vertices.push_back(x);
      out_vertices.push_back(y);
      out_vertices.push_back(z);

      // calculate uv
      float u = theta / (2 * M_PI);  
      float v = phi / M_PI;
      out_vertices.push_back(1-u);
      out_vertices.push_back(1-v);

      // calcualte normals (which is just the pos for a unit circle)
      out_vertices.push_back(x);
      out_vertices.push_back(y);
      out_vertices.push_back(z);
    }
  }

  // bottom pole
  out_vertices.push_back(0.0f);
  out_vertices.push_back(-1.0f);
  out_vertices.push_back(0.0f);
  out_vertices.push_back(0);
  out_vertices.push_back(0);
  out_vertices.push_back(0.0f);
  out_vertices.push_back(-1.0f);
  out_vertices.push_back(0.0f);

  // generate triangles
  // 1. Top pole
  for(size_t i=0; i<n_slices; i++){
    size_t i0 = i+1;
    size_t i1 = (i + 1) % n_slices + 1;
    out_indices.push_back(0);
    out_indices.push_back(i0);
    out_indices.push_back(i1);
  }

  // 2. Body
  for(size_t j=0; j<n_stacks-2; j++){
    size_t row0 = j * n_slices + 1; 
    size_t row1 = (j + 1) * n_slices + 1;
    for(size_t i=0; i<n_slices; i++){
      size_t i0 = row0 + i; 
      size_t i1 = row0 + (i + 1) % n_slices; 
      size_t i2 = row1 + (i + 1) % n_slices;
      size_t i3 = row1 + i;
      out_indices.push_back(i0);
      out_indices.push_back(i2);
      out_indices.push_back(i1);

      out_indices.push_back(i0);
      out_indices.push_back(i3);
      out_indices.push_back(i2);
    }
  }

  // 3. Bottom pole
  size_t n_verts = n_slices * (n_stacks - 1) + 2;
  size_t bottom_idx = n_verts - 1;
  size_t last_row = n_slices * (n_stacks - 2) + 1;
  for(size_t i=0; i < n_slices; i++){
    size_t i0 = last_row + i;
    size_t i1 = last_row + (i + 1) % n_slices;
    out_indices.push_back(bottom_idx);
    out_indices.push_back(i1);
    out_indices.push_back(i0);
  }

  
}

