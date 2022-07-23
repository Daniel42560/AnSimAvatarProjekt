#version 330

layout(location = 0) in vec4 position;
layout(location = 2) in vec2 uvCoord;

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

out vec2 passUVCoord;

void main() {
    gl_Position = projectionMatrix * viewMatrix * modelMatrix * position;
    passUVCoord = uvCoord * 3 - 1;
}