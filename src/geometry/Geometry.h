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

struct Capsule {
  // Generates a cylinder going from a to b, where the line drawn from a to b acts as the center for the radius
  static void generate_linecylinder(glm::vec3 a, glm::vec3 b, float radius, std::vector<glm::vec3>& outLineVerts, int curve_res = 24);
};
