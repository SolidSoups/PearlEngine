#pragma once

#include <vector>

constexpr int PLANE_VERTEX_SIZE = 32;
constexpr int PLANE_INDEX_SIZE = 6;

struct Plane{
  static const float vertices[PLANE_VERTEX_SIZE];
  static const unsigned int indices[PLANE_INDEX_SIZE];
};

struct Sphere{
  static void generate_uvsphere(std::vector<float> &out_vertices, 
                         std::vector<unsigned int> &out_indices,
                         int n_slices, int n_stacks);
};
