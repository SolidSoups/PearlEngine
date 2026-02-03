#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;
out mat3 TBN;   // Tangent, Bitangent, Normal (surface)

uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

void main(){
  vec4 worldPos = transform * vec4(aPos, 1.0);
  FragPos = worldPos.xyz;
  TexCoords = aTexCoords;

  // transform normal, tangent to world space
  mat3 normalMatrix = mat3(transpose(inverse(transform)));
  vec3 N = normalize(normalMatrix * aNormal);
  vec3 T = normalize(normalMatrix * aTangent);
  // re-orthogonalize T with respect to N // REMEMBER GRAM SCHMIDT
  T = normalize(T - dot(T, N) * N);
  // bitangent is perp. to both
  vec3 B = cross(N, T);
  TBN = mat3(T, B, N);
  Normal = N;

  gl_Position = projection * view * worldPos;
}
