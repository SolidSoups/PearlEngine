#version 330 core

out vec4 FragColor;
in vec3 FragPos;
in vec2 TexCoord;
in vec3 Normal;

void main(){
  float range = FragPos.y / 5.0f;
  FragColor = vec4(range, range, range, 1.0f);
}
