#version 330 core
layout (location=0) in vec3 aPosition; // the position coordinates
layout (location=1) in vec2 aTexCoord; // the uv coordinates
layout (location=2) in vec3 aNormal; // the normal coordinates 

// out vec3 FragColor; // specify a color output to the fragment shader
out vec2 TexCoord;
out vec3 NormalCoord;

uniform mat4 transform;
uniform mat4 projection;
uniform mat4 view;

void main(){
  gl_Position = projection * view * transform * vec4(aPosition, 1.0);
  // FragColor = aColor;
  TexCoord = aTexCoord;
  NormalCoord = aNormal;
}
