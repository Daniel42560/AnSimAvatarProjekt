#version 330 core

layout(location = 0) in vec3 positionAttribute;

uniform float time;

void main()
{
   vec3 pos = positionAttribute;
   pos.y = sin(time + pos.y);

   //gl_PointSize = 5.0f;
   gl_Position = vec4(pos, 1.0f);
}
