#version 330 core

in vec2 TexCoord; // texture coordinate
in vec3 NormalCoord;

uniform sampler2D mainTexture;

out vec4 FragColor;

void main(){
  // vec4 texColor = texture(mainTexture, TexCoord);
  vec4 texColor = vec4(NormalCoord.x, NormalCoord.y, NormalCoord.z, 1);
  FragColor = texColor;
}
