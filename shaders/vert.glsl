#version 330 core
layout (location=0) in vec3 aPosition; // vertex position in model space
layout (location=1) in vec2 aTexCoord; // texture coordinates
layout (location=2) in vec3 aNormal; // vertex normal in model space

out vec2 texCoord;
out vec3 normal;
out vec3 fragPosition;
out vec3 viewDirection;

uniform mat4 transform;
uniform mat4 projection;
uniform mat4 view;
uniform vec3 cameraPosition;

void main(){
  mat4 mvp = projection * view * transform;
  gl_Position = mvp * vec4(aPosition, 1.0);
  fragPosition = (transform * vec4(aPosition, 1.0)).xyz;

  // pass texture coordinates and normals to fragment shader
  texCoord = aTexCoord;
  normal = aNormal;

  // calculate direction from fragment to camera
  viewDirection = normalize(cameraPosition - fragPosition);
}
