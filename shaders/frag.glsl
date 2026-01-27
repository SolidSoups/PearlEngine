#version 330 core

in vec2 texCoord;
in vec3 normal;
in vec3 fragPosition;
in vec3 viewDirection;
in vec4 ambientLight;

uniform sampler2D albedoMap;
uniform sampler2D specularMap;

struct PointLight {
  vec3 position;
  vec3 color;
  float intensity;
  float range;
  float constant;
  float linear;
  float quadratic;
};

#define MAX_POINT_LIGHTS 16
uniform int numPointLights;
uniform PointLight pointLights[MAX_POINT_LIGHTS];

uniform vec4 light_diffuse = vec4(1.0, 0.9, 0.8, 1.0);
uniform vec4 light_specular = vec4(0.9, 0.8, 0.7, 1.0);
uniform vec3 light_position = vec3(10.0, 25.0, 15.0);
uniform vec3 light_attenuation = vec3(1.0, 0.1, 0.001);

uniform vec4 materialAmbient = vec4(1.0, 1.0, 1.0, 1.0);
uniform vec4 materialDiffuse = vec4(1.0, 1.0, 1.0, 1.0);
uniform vec4 materialSpecular = vec4(1.0, 1.0, 1.0, 1.0);
uniform int materialShininess = 8;

out vec4 FragColor;

vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 texColor, float specularIntensity) {
  vec3 lightDir = normalize(light.position - fragPos);
  float distance = length(light.position - fragPos);

  // Attenuation
  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

  // Diffuse
  float diffuseIntensity = max(dot(lightDir, normal), 0.0);
  vec3 diffuse = diffuseIntensity * light.color * light.intensity * materialDiffuse.xyz * texColor.xyz;

  // Specular (Blinn-Phong)
  vec3 specular = vec3(0.0);
  if (diffuseIntensity > 0.0) {
    vec3 halfVector = normalize(lightDir + viewDir);
    float specularBase = max(dot(halfVector, normal), 0.0);

    float specularPower = specularBase;
    for (int i = 0; i < materialShininess; ++i) {
      specularPower *= specularBase;
    }

    specular = specularPower * light.color * light.intensity * materialSpecular.xyz * specularIntensity;
  }

  return (diffuse + specular) * attenuation;
}

void main() {
  vec4 texColor = texture(albedoMap, texCoord);
  float specularIntensity = texture(specularMap, texCoord).r;

  // start with ambient
  FragColor = vec4((materialAmbient * ambientLight * texColor).xyz, texColor.a);

  // normalize interpolated normal
  vec3 normalizedNormal = normalize(normal);

  // ambient component
  FragColor.xyz += (materialAmbient * ambientLight * texColor).xyz;

  for (int i = 0; i < numPointLights && i < MAX_POINT_LIGHTS; i++) {
    FragColor.xyz += CalculatePointLight(
        pointLights[i],
        normalizedNormal,
        fragPosition,
        viewDirection,
        texColor.xyz,
        specularIntensity
      );
  }
}
