#version 330 core

in vec2 texCoord;
in vec3 normal;
in vec3 fragPosition;
in vec3 viewDirection;

uniform sampler2D albedoMap;
uniform sampler2D specularMap;

uniform vec4 light_ambient = vec4(0.2, 0.2, 0.2, 1.0);
uniform vec4 light_diffuse = vec4(1.0, 0.9, 0.8, 1.0);
uniform vec4 light_specular = vec4(0.9, 0.8, 0.7, 1.0);
uniform vec3 light_position = vec3(10.0, 25.0, 15.0);
uniform vec3 light_attenuation = vec3(1.0, 0.1, 0.001);

uniform vec4 materialAmbient = vec4(1.0, 1.0, 1.0, 1.0);
uniform vec4 materialDiffuse = vec4(1.0, 1.0, 1.0, 1.0);
uniform vec4 materialSpecular = vec4(1.0, 1.0, 1.0, 1.0);
uniform int materialShininess = 8;

out vec4 FragColor;

void main() {
  vec4 texColor = texture(albedoMap, texCoord);
  FragColor = vec4(0.0, 0.0, 0.0, texColor.a);
  float specularIntensity = texture(specularMap, texCoord).r;

  // ambient component
  FragColor.xyz += (materialAmbient * light_ambient).xyz;

  // normalize interpolated normal vector
  vec3 normalizedNormal = normalize(normal);

  // direction from fragment to light
  vec3 lightDirection = normalize(light_position - fragPosition);

  // diffuse component
  float diffuseIntensity = max(dot(lightDirection, normalizedNormal), 0.0);
  if (diffuseIntensity > 0.0) {
    vec3 diffuse = diffuseIntensity * light_diffuse.xyz * materialDiffuse.xyz;
    float distance = length(light_position - fragPosition);

    float attenuation = 1.0 / (light_attenuation.x + light_attenuation.y * distance + light_attenuation.z * pow(distance, 2.0));
    FragColor.xyz += diffuse * attenuation;

    // specular component (Blinn-Phong)
    vec3 halfVector = normalize(lightDirection + viewDirection);
    float specularBase = max(dot(halfVector, normalizedNormal), 0.0);

    float specularPower = specularBase;
    for(int i = 0; i < materialShininess; ++i){
      specularPower *= specularBase;
    }

    FragColor.xyz += specularPower * light_specular.xyz * attenuation * materialSpecular.xyz * specularIntensity;
  }
}
