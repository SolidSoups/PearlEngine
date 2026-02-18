#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in float aAxisType; // 0=grid, 1=X axis, 2=Z axis

out float vAxisType;

uniform mat4 view;
uniform mat4 projection;

void main() {
    vAxisType = aAxisType;
    gl_Position = projection * view * vec4(aPos, 1.0);
}
