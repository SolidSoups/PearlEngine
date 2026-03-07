#version 330 core
layout (location=0) out vec3 gPosition;
layout (location=1) out vec3 gNormal;
layout (location=2) out vec4 gAlbedoSpec;

in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;

uniform sampler2D uHeightMap;

void main(){
  gPosition = FragPos;
  vec3 color = texture(uHeightMap, TexCoord).rgb;
  gAlbedoSpec.rgb = vec3(color.r, color.r, color.r);
  gAlbedoSpec.a = 0.0;
  gNormal = Normal * 0.5 + 0.5;
}
