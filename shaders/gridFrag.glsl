#version 330 core
in float vAxisType;
out vec4 fragColor;

uniform vec4 lineColor;
uniform vec4 xAxisColor;
uniform vec4 zAxisColor;

void main() {
    if (vAxisType < 0.5)
        fragColor = lineColor;
    else if (vAxisType < 1.5)
        fragColor = xAxisColor;
    else
        fragColor = zAxisColor;
}
