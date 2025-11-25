#version 330 core

in vec2 TexCoord; // texture coordinate
in vec3 NormalCoord;

uniform sampler2D mainTexture;

out vec4 FragColor;

float map(float value, float min1, float max1, float min2, float max2) {
    return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

vec4 mapVec(vec4 vec, float min1, float max1, float min2, float max2){
  return vec4(
      map(vec.x, min1, max1, min2, max2),
      map(vec.y, min1, max1, min2, max2),
      map(vec.z, min1, max1, min2, max2),
      map(vec.w, min1, max1, min2, max2)
      );
}

void main(){
  vec4 texColor = texture(mainTexture, TexCoord);
  // vec4 texColor = vec4(NormalCoord.x, NormalCoord.y, NormalCoord.z, 1);
  FragColor = texColor;
}
