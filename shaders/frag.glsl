#version 330 core

in vec3 vertexColor; // the input variable from the vertex shader (same name and type)
in vec2 TexCoord; // texture coordinate

uniform sampler2D mainTexture;

out vec4 FragColor;

void main(){
  vec4 texColor = texture(mainTexture, TexCoord);
  FragColor = texColor;
}
