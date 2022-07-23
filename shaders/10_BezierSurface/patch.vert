#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 model;

out vec3 color;

out vec2 passUv;

void main() {

    passUv = uv;

    // since we are only dealing with points, we can just shade in the vertex
    // shader!

    // light source fixed in world space
    vec4 lightPosW = vec4(10.0, 10.0, 10.0, 1.0);

    // viewspace light
    vec3 lightPosV = (view * lightPosW).xyz;

    mat4 mv = view * model;
    mat3 normalmatrix = transpose(inverse(mat3(mv)));

    // viewspace point
    vec3 posV = (mv * vec4(position, 1.)).xyz;
    // viewspace normalized normal
    vec3 NV = normalize(normalmatrix * normal);

    // viewspace normalized light vector
    vec3 LV = normalize(lightPosV - posV);

    // simple diffuse phong
    color = vec3(1.0, 0.0, 0.0) * max(0.0, dot(LV, NV));

    gl_Position = projection * view * model * vec4(position, 1);
}
