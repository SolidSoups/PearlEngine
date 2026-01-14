#version 330 core

in vec2 TexCoord; // texture coordinate
in vec3 NormalCoord;
in vec3 position;
in vec3 cameraDir;

uniform sampler2D albedoMap;
uniform sampler2D specularMap;

// some defaults
uniform vec4 primaryColorVec4 = vec4(1.0, 1.0, 1.0, 1.0);
uniform vec4 light_ambient = vec4(0.2, 0.2, 0.2, 1.0);
uniform vec4 light_diffuse = vec4(1, 0.9, 0.8, 1.0);
uniform vec4 light_specular = vec4(0.9, 0.8, 0.7, 1.0);
uniform vec3 light_position = vec3(10, 25, 15);
uniform vec3 light_attenuation = vec3(1.0, 0.1, 0.001);

uniform vec4 materialAmbient = vec4(1, 1, 1, 1);
uniform vec4 materialDiffuse = vec4(1, 1, 1, 1);
uniform vec4 materialSpecular = vec4(1, 1, 1, 1);
uniform int materialShininess = 8;

out vec4 FragColor;

void main() {
  vec4 texColor = texture(albedoMap, TexCoord);
  FragColor = vec4(0, 0, 0, texColor.w);
  float specular = texture(specularMap, TexCoord).r;

  // ambient first
  FragColor.xyz += (materialAmbient * light_ambient).xyz;

  // diffuse part first
  vec3 lightDirection = normalize(position.xyz - light_position.xyz);

  // interpolated mesh may not be interpolated, fix it
  vec3 normalizedNormal = normalize(vec3(NormalCoord.xyz));

  float difIntensity = max(dot(lightDirection, normalizedNormal), 0.0);
  if (difIntensity > 0) {
    vec3 diffusePart = difIntensity * light_diffuse.xyz * materialDiffuse.xyz;
    float distance = length(light_position.xyz - position.xyz);

    float attenuation = 1 / (light_attenuation.x + light_attenuation.y * distance + light_attenuation.z * pow(distance, 2));
    FragColor.xyz += diffusePart * attenuation;

    // specular part
    vec3 normCamaraDir = normalize(cameraDir);

    vec3 halfVector = normalize((-lightDirection + normCamaraDir) / 2);
    float initialBrightness = max(dot(halfVector, normalizedNormal), 0.0);

    float totalBrightness = initialBrightness;
    for(int i=0; i<materialShininess; ++i){
      totalBrightness *= initialBrightness;
    }

    // add the specular part, taking
    FragColor.xyz += totalBrightness * light_specular.xyz * attenuation * materialSpecular.xyz;
  }
}
