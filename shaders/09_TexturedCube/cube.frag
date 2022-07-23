#version 330

in vec2 passUVCoord;

uniform sampler2D tex0;
uniform sampler2D tex1;

out vec4 fragmentColor;

void main() {

    vec4 color = vec4(texture(tex0, passUVCoord).rgb, 1.0f);
    vec4 color2 = vec4(texture(tex1, passUVCoord).rgb, 1.0f);
    fragmentColor = color * color2;
}