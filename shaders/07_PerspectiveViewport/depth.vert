#version 330 core

in vec4 positionAttribute;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec4 passPosition;

void main() {

    gl_Position =
        projectionMatrix * viewMatrix * modelMatrix * positionAttribute;
    passPosition = viewMatrix * modelMatrix * positionAttribute;
}