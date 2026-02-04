#pragma once
#include <nlohmann/json.hpp>
using json = nlohmann::json;
#include <glm/glm.hpp>

namespace glm {
inline void to_json(json &j, const vec3 &v) { j = {v.x, v.y, v.z}; }
inline void to_json(json &j, const vec4 &v) { j = {v.x, v.y, v.z, v.w}; }
inline void to_json(json &j, const mat4 &m) { 
  j = json::array();
  for(int i=0; i<4; i++)
    j.push_back({ m[i].x, m[i].y, m[i].z, m[i].w });
}
inline void from_json(const json &j, vec3 &v) {
  v.x = j[0];
  v.y = j[1];
  v.z = j[2];
}
inline void from_json(const json &j, vec4 &v){
  v.x = j[0];
  v.y = j[1];
  v.z = j[2];
  v.w = j[3];
}
inline void from_json(const json &j, mat4 &m){
  for(int i=0; i<4; i++){
    m[i].x = j[i][0];
    m[i].y = j[i][1];
    m[i].z = j[i][2];
    m[i].w = j[i][3];
  }
}
} // namespace glm
