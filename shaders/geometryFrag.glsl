
#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;
in mat3 TBN;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;

void main(){
  gPosition = FragPos;
  gAlbedoSpec.rgb = texture(texture_diffuse1, TexCoords).rgb;
  // gAlbedoSpec.rgb = vec3(1.0, 0.0, 0.0);
  gAlbedoSpec.a = texture(texture_specular1, TexCoords).r;


  // sample map (stored as 0-1, converted to -1 to 1)
  vec3 normal = texture(texture_normal1, TexCoords).rgb * 2.0 - 1;
  // transform from tangent space to world space
  normal = normalize(TBN * normal);
  // pack back to 0-1 range in G-Buffer
  gNormal = normal * 0.5 + 0.5;
}



