#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUV;
layout(location = 2) in vec3 aNormal;

out vec3 vWorldPos;
out vec3 vCameraPos;

uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPos;

void main() {
  vWorldPos = aPos;
  vCameraPos = cameraPos;
  gl_Position = projection * view * vec4(aPos, 1.0);
}
