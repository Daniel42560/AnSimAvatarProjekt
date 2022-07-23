#version 330 core

out vec3 fragmentColor;

uniform sampler2D tex;

in vec3 color;

in vec2 passUv;

void main() { fragmentColor = color * texture(tex, passUv).xyz; }
