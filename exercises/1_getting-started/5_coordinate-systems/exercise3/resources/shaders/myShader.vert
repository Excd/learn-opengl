#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec2 vTexCoord;

out vec2 fTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    // Matrix multiplication is performed right to left.
    gl_Position = projection * view * model * vec4(vPos, 1.0);
    fTexCoord = vTexCoord;
}