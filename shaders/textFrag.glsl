#version 330 core

out vec4 FragColor;

in vec2 FragPos;
in vec2 TexCoord;

uniform sampler2D uTextAtlas;

void main(){
  FragColor = texture(uTextAtlas, TexCoord); 
}
