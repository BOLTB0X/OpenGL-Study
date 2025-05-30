#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float thickness;

void main(void)
{
    vec3 offsetPos = aPos + aNormal * thickness;
    gl_Position = projection * view * model * vec4(offsetPos, 1.0);
}