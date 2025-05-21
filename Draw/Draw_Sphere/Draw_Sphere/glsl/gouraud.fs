#version 330 core
in vec3 VertexColor;
out vec4 FragColor;

void main(void)
{
    FragColor = vec4(VertexColor, 1.0);
}