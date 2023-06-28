#version 330 core

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vColor;

out vec3 fColor;

uniform float xOffset;

void main() {
	gl_Position = vec4(vPos.x + xOffset, vPos.yz, 1.0);
	fColor = vColor;
}