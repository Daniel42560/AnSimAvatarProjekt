#version 330 core

out vec4 fragmentColor;

in vec4 passPosition;

uniform float near;
uniform float far;

void main() {

    float depth = -passPosition.z;
    depth = (depth - near) / (far - near);

    fragmentColor = vec4(vec3(depth), 1);
}
