#version 330 core

layout(location = 0) in vec3 positionAttribute;
layout(location = 1) in vec3 normalAttribute;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec3 passPositionCamera;
out vec3 passNormalCamera;

void main() {
    mat4 mv = view * model;
    // Füge Positionsattribut (vec3) homogene Koordinate hinzu (Punkt)
    gl_Position = projection * mv * vec4(positionAttribute, 1.0);

    passPositionCamera = (mv * vec4(positionAttribute, 1.0)).xyz;
    passNormalCamera = transpose(inverse(mat3(mv))) * normalAttribute;
}
