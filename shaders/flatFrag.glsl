#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gDepthStencil;

void main(){
  vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;
  FragColor = vec4(Albedo, 1.0);
}

