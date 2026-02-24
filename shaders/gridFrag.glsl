#version 330 core
out vec4 fragColor;

in vec3 vWorldPos;
in vec3 vCameraPos;

uniform vec4 lineColor = vec4(1.0, 1.0, 1.0, 0.3);
uniform vec4 xAxisColor = vec4(1.0, 0.0, 0.0, 1.0);
uniform vec4 zAxisColor = vec4(0.0, 0.0, 1.0, 1.0);
uniform float innerRadius = 400.0f;
uniform float outerRadius = 900.0f;

float gridLine(float coord, float thickness){
  float fw = fwidth(coord);
  return 1.0 - clamp(
                abs(fract(coord - 0.5) - 0.5) / (fw * thickness),
                0.0,
                1.0
              );
}

void main() {
  float gx = gridLine(vWorldPos.x, 0.8);
  float gz = gridLine(vWorldPos.z, 0.8);

  bool onXAxis = gx > 0.001 && abs(vWorldPos.x) < 0.5;
  bool onZAxis = gz > 0.001 && abs(vWorldPos.z) < 0.5;

  // determine color
  vec4 color = vec4(0.0f);
  if(onXAxis)
    color = xAxisColor;
  else if(onZAxis)
    color = zAxisColor;
  else
    color = lineColor;

  // determine fade
  float dist = length(vWorldPos.xz - vCameraPos.xz);
  float fade = 1.0 - smoothstep(innerRadius, outerRadius, dist);

  fragColor = vec4(color.rgb, color.a * max(gx, gz) * fade);
}
