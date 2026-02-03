#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gDepthStencil;

struct PointLight{
  vec3 position;
  vec3 color;
  float intensity;
  float range;
  float constant;
  float linear;
  float quadratic;
};

const int MAX_POINT_LIGHTS = 8;
uniform int numPointLights;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform vec3 viewPos;

uniform vec4 ambientColor;
uniform float ambientIntensity;
uniform int materialShininess = 8;

vec3 CalculatePointLight(PointLight light, vec3 pos, vec3 normal, vec3 color, float specular, vec3 viewDir){
  // light direction and distance
  vec3 lightDir = light.position - pos;
  float distance = length(lightDir);
  lightDir = normalize(lightDir);

  float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);
  float window = smoothstep(light.range, 0, distance);
  float final = attenuation * window;

  // diffuse
  float diffuseIntensity = max(dot(lightDir, normal), 0.0);
  vec3 diffuse = diffuseIntensity * light.color * light.intensity * color;

  vec3 specularOut = vec3(0.0);
  if(diffuseIntensity > 0.0){
    vec3 halfVector = normalize(lightDir + viewDir);
    float specularBase = max(dot(halfVector, normal), 0.0);

    float specPower = specularBase;
    for(int i=0; i<materialShininess; ++i){
      specPower *= specularBase;
    }

    specularOut = specPower * light.color * light.intensity * specular;
  }
  return (diffuse + specularOut) * final;
}

void main(){
  // retrieve data from G-buffer
  vec3 FragPos = texture(gPosition, TexCoords).rgb;
  vec3 Normal = texture(gNormal, TexCoords).rgb * 2.0 - 1.0; // normal needs to be -1 to 1 range
  vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;
  float Specular = texture(gAlbedoSpec, TexCoords).a;
  float Depth = texture(gDepthStencil, TexCoords).r;

  // calculate lighting as usual
  vec3 ambient = ambientColor.rgb * ambientIntensity;
  vec3 lighting = Albedo * ambient; // hard-coded ambient comp
  vec3 viewDir = normalize(viewPos - FragPos);
  for(int i=0; i < MAX_POINT_LIGHTS && i < numPointLights; ++i){
    lighting += CalculatePointLight(pointLights[i], FragPos, Normal, Albedo, Specular, viewDir);
  }

  FragColor = vec4(lighting, 1.0);
}

