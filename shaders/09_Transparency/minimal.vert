#version 330 core

in vec4 positionAttribute;

uniform mat4 M;

void main() { gl_Position = M * positionAttribute; }
