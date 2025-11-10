#version 330 core
layout (location=0) in vec3 aPos; // the position variable has attribute position 0
layout (location=1) in vec3 aColor; // the color variable has attribute position
layout (location=2) in vec2 aTexCoord;

out vec3 FragColor; // specify a color output to the fragment shader
out vec2 TexCoord;

uniform mat4 transform;
uniform mat4 projection;
uniform mat4 view;

void main(){
  gl_Position = projection * view * transform * vec4(aPos, 1.0);
  FragColor = aColor;
  TexCoord = aTexCoord;
}
