#version 330 core
layout (location=0) in vec2 aPosition;
layout (location=1) in vec2 aTexCoord;

out vec2 FragPos;
out vec2 TexCoord;

uniform mat4 ortho;
uniform mat4 model;

void main() {
  vec4 screenPos = ortho * model * vec4(aPosition, 0.0, 1.0);  
  FragPos = screenPos.xy;
  TexCoord = aTexCoord;
  
  gl_Position = screenPos;
}
