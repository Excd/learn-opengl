#version 330 core

in vec2 fTexCoord;

layout (location = 0) out vec4 color;

uniform sampler2D textures[2];
uniform float mixValue;

void main() {
    color = mix(texture(textures[0], fTexCoord), texture(textures[1], fTexCoord), mixValue);
}