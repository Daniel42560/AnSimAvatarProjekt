#version 330 core

layout(location = 1) in vec3 positionAttribute;
layout(location = 0) in vec3 velocityAttribute;

uniform float time;

void main()
{
   vec3 velocity = velocityAttribute;
   vec3 pos = positionAttribute;
   //pos.y = sin(time + pos.y);
   /*
   pos.x += velocity.x * time;
   pos.y += velocity.y * time;
   pos.z += velocity.z * time;
   */

   //gl_PointSize = 5.0f;
   gl_Position = vec4(pos, 1.0f);
}
