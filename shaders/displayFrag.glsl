#version 330 core

in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D textureSampler;
uniform int bufferType; // 0=position, 1=normal, 2=albedo, 3=depth

void main() {
  if (bufferType == 3) {
    float depth = texture(textureSampler, TexCoords).r;
    float z = depth * 2.0 - 1.0; // convert to NDC
    float linearDepth = (2.0 * 0.1 * 100.0) / (100.0 + 0.1 - z * (100.0 - 0.1));
    linearDepth = linearDepth / 100.0; // Normalize to 0-1 range
    FragColor = vec4(linearDepth, linearDepth, linearDepth, 1.0);
  }
  else {
    vec3 color = texture(textureSampler, TexCoords).rgb;
    FragColor = vec4(color, 1.0);
  }
}
