#version 330 core

in vec2 TexCoord; // texture coordinate
in vec3 NormalCoord;

uniform sampler2D albedoMap;
uniform sampler2D specularMap;

out vec4 FragColor;

void main() {
  vec4 texColor = texture(albedoMap, TexCoord);
  float specular = texture(specularMap, TexCoord).r;
  // vec4 texColor = vec4(NormalCoord.x, NormalCoord.y, NormalCoord.z, 1);
  FragColor = texColor;
}
