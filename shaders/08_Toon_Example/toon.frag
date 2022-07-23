#version 330 core

out vec3 fragmentColor;

uniform vec3 lightPosition;
uniform mat4 view;

uniform vec3 color;

in vec3 passPositionCamera;
in vec3 passNormalCamera;

void main() {

    vec4 lightPositionCamera = view * vec4(lightPosition, 1.0);
    vec3 L = normalize(lightPositionCamera.xyz - passPositionCamera);
    vec3 N = normalize(passNormalCamera);

    float cosPhi = max(dot(N, L), 0.0);

    float intensity = cosPhi;

    if (intensity < 0.2) {
        intensity = 0.0;
    } else if (intensity < 0.5) {
        intensity = 0.2;
    } else if (intensity < 0.7) {
        intensity = 0.5;
    } else if (intensity < 0.9) {
        intensity = 0.7;
    } else {
        intensity = 1.0;
    }

    fragmentColor = intensity * color;
}
