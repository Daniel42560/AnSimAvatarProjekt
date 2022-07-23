#version 330 core

in vec4 passPosition;
in vec3 passNormal;

uniform vec3 lightPosition;

out vec4 fragmentColor;

void main() {
    // compute the light vector as the normalized vector between
    // the vertex position and the light position:
    vec3 lightVector = normalize(lightPosition - passPosition.xyz);

    // compute the eye vector as the normalized negative vertex position in
    // camera coordinates:
    vec3 eye = normalize(-passPosition.xyz);

    vec3 N = normalize(passNormal);

    // compute the normalized reflection vector using GLSL's built-in reflect()
    // function:
    vec3 reflection = normalize(reflect(-lightVector, N));

    // variables used in the phong lighting model:
    float phi = max(dot(N, lightVector), 0);
    float psi = pow(max(dot(reflection, eye), 0), 15);

    vec3 ambientColor = vec3(0.3, 0.2, 0.2);
    vec3 diffuseColor = vec3(1.0, 0.0, 0.0);
    vec3 specularColor = vec3(1.0, 1.0, 1.0);

    fragmentColor =
        vec4(ambientColor + phi * diffuseColor + psi * specularColor, 1);
}
