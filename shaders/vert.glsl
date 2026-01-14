#version 330 core
layout (location=0) in vec3 aPosition; // the position coordinates
layout (location=1) in vec2 aTexCoord; // the uv coordinates
layout (location=2) in vec3 aNormal; // the normal coordinates 

// out vec3 FragColor; // specify a color output to the fragment shader
out vec2 TexCoord;
out vec3 NormalCoord;
out vec3 position;
out vec3 cameraDir;

uniform mat4 transform;
uniform mat4 projection;
uniform mat4 view;
uniform vec4 camera;

void main(){
  mat4 mvp = projection * view * transform; 
  gl_Position = mvp * vec4(aPosition, 1.0);
  position = (transform * vec4(aPosition, 1)).xyz;

  // pass along
  TexCoord = aTexCoord;
  NormalCoord = aNormal;

  // calculate vector to eye from vertex
  cameraDir = normalize((camera.xyz - position.xyz));
}
